#ifndef DEV_MODEL_H
#define DEV_MODEL_H

#include <string>

#include <Element/DGroup.h>
#include <Model/DMesh.h>
#include <Core/DLog.h>

namespace dev
{
  class Model :
    public Group
  {
  public:
    enum FileID
    {
      MaxFile = 0xF1A2B400,
      ModelFile = 0xF1A2B401
    };

    enum MeshID
    {
      VertexMesh =  0xE1A2B401,
      Index32Mesh = 0xE1A2B402,
      Index16Mesh = 0xE1A2B403
    };

    Model(const wchar_t* path, const Vec3& position, const Vec3& rotation, const Vec3& scale);
    Model(const wchar_t* path);
    virtual ~Model();

    bool OpenFile(const wchar_t* path);
    
    inline const wchar_t* GetFileName() const
    {
      return _path.c_str();
    }
    
    inline DWORD GetMeshCount() const 
    {
      return _meshCount;
    }

    inline Mesh* GetMesh(const wchar_t* name) const
    {
      for(UINT i = 0; i < _elements.size(); i++)
      {
        Mesh* mesh = _elements[i]->AsMesh();
        if (mesh && !wcscmp(mesh->GetName(), name))
          return mesh;
      }
      return NULL;
    }

    inline Mesh* GetMeshByNum(UINT num) const
    {
      if (_elements.size() > num)
        return _elements[num]->AsMesh();

      return NULL;
    }

    bool IsLoaded() { return _loaded; }

    virtual inline Model* AsModel() 
    {
      return this;
    }
  
  protected:
    std::wstring _path;

    DWORD _meshCount;
    bool _loaded;
  };
}

#endif