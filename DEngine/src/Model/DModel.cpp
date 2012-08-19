#include <Model/DModel.h>

using namespace dev;

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
  
  HANDLE hFile = CreateFileW(path, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
  if (hFile == INVALID_HANDLE_VALUE)
  {
    std::wstring buffer(L"File not loaded: ");
    buffer += path;
    Log::GetLog()->WriteToLog(buffer.c_str());
    return false;
  }

  int fileSize = GetFileSize(hFile, 0) - 3 * sizeof(DWORD);

  DWORD bIndex = 0;
  if (!readFile(hFile, &bIndex, sizeof(DWORD)))
    return FALSE;

  if (bIndex != Index16File && bIndex != Index32File && bIndex != VertexFile)
  {
    CloseHandle(hFile);
    std::wstring buffer(L"File not loaded: ");
    buffer += _path;
    Log::GetLog()->WriteToLog(buffer.c_str());
    return FALSE;
  }

  if (!readFile(hFile, &_meshCount, sizeof(DWORD)))
    return FALSE;

  Buffer::BufferType typeVertex = Buffer::BT_NONE;
  if (!readFile(hFile, &typeVertex, sizeof(DWORD)))
    return FALSE;

  DWORD _vNumber = 0;
  DWORD _fNumber = 0;
  Mesh* mesh = NULL;

  for (DWORD i = 0; i < _meshCount; i++)
  {
    //read name
    {
      DWORD strLen = 0;
      if (!readFile(hFile, &strLen, sizeof(DWORD)))
        return FALSE;

      char* name = new char[strLen];
      if (!readFile(hFile, name, strLen))
      {
        delete[] name;
        return FALSE;
      }

      wchar_t* meshName = new wchar_t[strLen];
      MultiByteToWideChar(CP_ACP, 0, name, strLen, meshName, strLen);
      mesh = new Mesh(meshName);
      delete[] meshName;
      delete[] name;
    }

    //read faces and vertices number
    {
      if (!readFile(hFile, &_vNumber, sizeof(DWORD)))
      {
        delete mesh;
        return FALSE;
      }

      if (!readFile(hFile, &_fNumber, sizeof(DWORD)))
      {
        delete mesh;
        return FALSE;
      }
    }

    //Read vertices
    {
      if (bIndex == VertexFile)
        _vNumber = _fNumber * 3;

      int sizeBuffer = _vNumber * Buffer::Declaration::GetSizeBuffer(typeVertex);
      char* buffer = new char[sizeBuffer];
      if (!readFile(hFile, buffer, sizeBuffer))
      {
        delete mesh;
        delete[] buffer;
        return FALSE;
      }

      mesh->SetVertices(buffer, _vNumber, typeVertex);
      delete[] buffer;
    }
    
    //Read indexes
    {
      Buffer::BufferType indexBufferType = Buffer::BT_NONE;

      switch(bIndex)
      {
      case Index16File:
        indexBufferType = Buffer::IB_16;
        break;
      case Index32File:
        indexBufferType = Buffer::IB_32;
        break;
      }

      int sizeBuffer = _fNumber * 3 * Buffer::Declaration::GetSizeBuffer(indexBufferType);
      char* buffer = new char[sizeBuffer];
      if (!readFile(hFile, buffer, sizeBuffer))
      {
        delete mesh;
        delete[] buffer;
        return FALSE;
      }

      mesh->SetIndexes(buffer, _fNumber * 3, indexBufferType);
      delete[] buffer;
    } 

    AddElement(mesh);
  }

  CloseHandle(hFile);
  return true;
}

bool Model::readFile(HANDLE hFile, LPVOID buffer, DWORD size)
{
  DWORD ReadValue = 0;
  ReadFile(hFile, buffer,  size, &ReadValue, 0);
  if (size != ReadValue)
  {
    std::wstring buffer(L"File not loaded: ");
    buffer += _path;
    Log::GetLog()->WriteToLog(buffer.c_str());
    return FALSE;
  }
  return TRUE;
}
