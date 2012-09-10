#include <Model/DModel.h>

#include <Utils/DFile.h>

using namespace dev;

#define BLOCKS_NUM  40

typedef std::set<UINT> Block;
typedef std::map<UINT, Block> Blocks;

Model::Model(const wchar_t* path, const Vec3& position, const Vec3& rotation, const Vec3& scale) :
  Group(position, rotation, scale),
  _path(L""),
  _meshCount(0),
  _loaded(false)
{
  OpenFile(path);
}

Model::Model(const wchar_t* path) :
  Group(),
  _path(L""),
  _meshCount(0),
  _loaded(false)
{
  OpenFile(path);
}

Model::~Model()
{
}

bool Model::OpenFile(const wchar_t* path)
{
  RemoveAllElements();

  _path = path;

  File file(path);
  if (!file.IsOpened())
    return FALSE;

  DWORD& fType = file.ReadCurrentInfo<DWORD>();
  DWORD fileType = fType;

  if (fileType != MaxFile && fileType != ModelFile)
  {
    WRITE_FILE_LOG(L"File not loaded: ", _path);
    return FALSE;
  }

  if (fileType == MaxFile)
    fType = ModelFile;

  _meshCount = file.ReadCurrentInfo<DWORD>();
  Buffer::BufferType& typeVertex = file.ReadCurrentInfo<Buffer::BufferType>();

  DWORD iNumber = 0;
  Mesh* mesh = NULL;

  file.SetCurrentWritePointer();

  for (DWORD i = 0; i < _meshCount; i++)
  {
    DWORD readPointer = file.GetCurrentReadPointer();
    //read name
    DWORD& strLen = file.ReadCurrentInfo<DWORD>();
    char* name = (char*)file.ReadCurrentInfo(strLen);
    if (name)
    {
      wchar_t* meshName = new wchar_t[strLen];
      MultiByteToWideChar(CP_ACP, 0, name, strLen, meshName, strLen);
      mesh = new Mesh(meshName);
      delete[] meshName;
    }
    //read faces and vertices number
    DWORD  meshType = file.ReadCurrentInfo<DWORD>();
    DWORD  vNumber  = file.ReadCurrentInfo<DWORD>();
    DWORD  iNumber  = file.ReadCurrentInfo<DWORD>();
    Vec3 bbMax;
    Vec3 bbMin;
    if (fileType == MaxFile)
    {
      bbMax = file.ReadCurrentInfo<Vec3>();
      bbMin = file.ReadCurrentInfo<Vec3>();
    }

    //Read vertices
    {
      UINT sizeBuffer = vNumber * Buffer::Declaration::GetSizeBuffer(typeVertex);
      byte* buffer = file.ReadCurrentInfo(sizeBuffer);
      if (!buffer)
        return FALSE;

      if (fileType != MaxFile)
      {
        mesh->SetVertices(buffer, vNumber, typeVertex);
      }
      else
      {
        file.RewriteInfo(readPointer, strLen + sizeof(DWORD));
        readPointer += strLen + sizeof(DWORD);

        int sizeVertex = Buffer::Declaration::GetSizeBuffer(typeVertex);
        Vec3 bs = (bbMax - bbMin) / BLOCKS_NUM;
        bs.x = 1 / bs.x;
        bs.y = 1 / bs.y;
        bs.z = 1 / bs.z;

        UINT* indexes = new UINT[vNumber];
        char* copyBuffer = new char[sizeBuffer];
        DWORD vertexNum = 0;

        {
          Blocks blocks;
          for (UINT i = 0; i < vNumber; i++)
          {
            UINT index = i * sizeVertex;
            Vec3& pos = (Vec3&)buffer[index];
            UINT numBlock = (UINT)((pos.x - bbMin.x) * bs.x) + BLOCKS_NUM * (UINT)((pos.y - bbMin.y) * bs.y) + BLOCKS_NUM * BLOCKS_NUM * (UINT)((pos.z - bbMin.z) * bs.z);
            Blocks::iterator it = blocks.find(numBlock);
            if (it != blocks.end())
            {
              Block& block = (*it).second;
              Block::iterator bIt = block.begin();
              for (; bIt != block.end(); ++bIt)
              {
                UINT& j = (*bIt);
                if (memcmp(&buffer[index], &buffer[j * sizeVertex], sizeVertex) == 0)
                {
                  indexes[i] = indexes[j];
                  break;
                }
              }
              if (bIt == block.end())
                it = blocks.end();
            }
            else
              blocks[numBlock] = Block();

            if (it == blocks.end())
            {
              blocks[numBlock].insert(i);
              memcpy(&copyBuffer[vertexNum * sizeVertex], &buffer[index], sizeVertex);
              indexes[i] = vertexNum++;
            }
          }
        }

        iNumber = vNumber;
        vNumber = vertexNum;

        DWORD sizeIndex = sizeof(UINT);
        if (iNumber <= USHRT_MAX)
          sizeIndex = sizeof(USHORT);

        if (sizeBuffer > (sizeVertex * vNumber + sizeIndex * iNumber))
        {
          if (iNumber <= USHRT_MAX)
            meshType = Index16Mesh;
          else
            meshType = Index32Mesh;
          file.WriteInfo(meshType);
          file.WriteInfo(vNumber);
          file.WriteInfo(iNumber);
        }
        else
        {
          file.RewriteInfo(readPointer, 3 * sizeof(DWORD));
        }

        mesh->SetVertices(copyBuffer, vNumber, typeVertex);
        file.WriteInfo(copyBuffer, vNumber * sizeVertex);
        
        if (meshType == Index16Mesh)
        {
          USHORT* iWord = new USHORT[iNumber];
          for (UINT i = 0; i < iNumber; i++)
            iWord[i] = indexes[i];
          mesh->SetIndexes(iWord, iNumber, Buffer::IB_16);
          file.WriteInfo(iWord, iNumber * sizeIndex);
        }
        else
        {
          mesh->SetIndexes(indexes, iNumber, Buffer::IB_32);
          file.WriteInfo(indexes, iNumber * sizeIndex);
        }
        meshType = 0;
        delete[] indexes;
        delete[] copyBuffer;
      }
    }
    
    //Read indexes
    if (meshType == Index16Mesh || meshType == Index32Mesh)
    {
      Buffer::BufferType indexBufferType = Buffer::BT_NONE;

      switch(meshType)
      {
      case Index16Mesh:
        indexBufferType = Buffer::IB_16;
        break;
      case Index32Mesh:
        indexBufferType = Buffer::IB_32;
        break;
      }

      int sizeBuffer = iNumber * Buffer::Declaration::GetSizeBuffer(indexBufferType);
      byte* buffer = file.ReadCurrentInfo(sizeBuffer);

      mesh->SetIndexes(buffer, iNumber, indexBufferType);
    }

    AddElement(mesh);
  }
  
  if (fileType == MaxFile)
    file.WriteEndFile();
  
  return TRUE;
}
