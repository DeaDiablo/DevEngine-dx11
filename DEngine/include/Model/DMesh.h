#ifndef DEV_MESH_H
#define DEV_MESH_H

#include <stdio.h>
#include <Element/DElement.h>
#include <DX/DirectX.h>

namespace dev
{
  class Mesh :
    public Element
  {
  public:
    Mesh(const wchar_t* name, const Vec3& position, const Vec3& rotation, const Vec3& scale);
    Mesh(const wchar_t* name);
    virtual ~Mesh();

    void SetVertices(LPVOID vertices, int numberVertex, Buffer::BufferType BT_Type);
    void SetIndexes(LPVOID indexes, int numberIndexes, Buffer::BufferType BT_Type);
    
    virtual inline Mesh* AsMesh() 
    {
      return this;
    }

    /*void SetMaterial(Material* material);
    inline Material* GetMaterial() const
    {
      return _material;
    }*/

    void RemoveTexture(const wchar_t* path);
    void SetTexture(const wchar_t* path);
    inline const TextureList& GetTextureList() const
    {
      return _textures;
    }

    inline const wchar_t* GetName() const
    {
      return _name.c_str();
    }

    inline void SetOrderNum(int num)
    {
      _orderNum = num;
    }
    inline int GetOrderNum() const
    {
      return _orderNum;
    }

  protected:
    virtual void updateParameters();
    virtual void draw();
    virtual void returnParameters();

    void dirtyTextures();

    std::wstring            _name;
    int                     _orderNum;

    Buffer::VertexBuffer*   _vertices;
    Buffer::IndexBuffer*    _indexes;

    //Material*               _material;
    TextureList             _textures;
    ID3D11SamplerState*     _sampler;
  };

  typedef std::vector<Mesh*> MeshList;
}

#endif