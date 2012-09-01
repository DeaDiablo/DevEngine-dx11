#include <Model/DMesh.h>

#include <DX/DirectX.h>

using namespace dev;

Mesh::Mesh(const wchar_t* name, const Vec3& position, const Vec3& rotation, const Vec3& scale) :
  Element(position, rotation, scale),
  _name(name),
  _vertices(NULL),
  _indexes(NULL)
  //_material(NULL)
{
  dirtyTextures();
  _sampler = DX.GetSamplerState();
}

Mesh::Mesh(const wchar_t* name) :
  Element(),
  _name(name),
  _vertices(NULL),
  _indexes(NULL)
  //_material(NULL)
{
  dirtyTextures();
  _sampler = DX.GetSamplerState();
}

Mesh::~Mesh()
{
  if (_indexes)
  {
    delete _indexes;
    _indexes = NULL;
  }

  if (_vertices)
  {
    delete _vertices;
    _vertices = NULL;
  }
  
  dirtyTextures();
  /*if (_material)
  {
    delete _material;
    _material = NULL;
  }*/
}

void Mesh::SetVertices(LPVOID vertices, int numberVertex, Buffer::BufferType BT_Type)
{
  if (_vertices)
    delete _vertices;
  _vertices = new Buffer::VertexBuffer(vertices, numberVertex, BT_Type);
  setBufferType(BT_Type);
}

void Mesh::SetIndexes(LPVOID indexes, int numberIndexes, Buffer::BufferType BT_Type)
{
  if (_indexes)
    delete _indexes;
  _indexes = new Buffer::IndexBuffer(indexes, numberIndexes, BT_Type);
}

void Mesh::draw()
{
  if (!_vertices)
    return;

  Element::draw();

  for(UINT i = 0; i < _textures.size(); i++)
    _textures[i]->SetPSTexture(i);

  DX_CONTEXT.PSSetSamplers(0, 1, &_sampler);

  _vertices->SetAsSource();
  if (_indexes == NULL)
  {
    DX_CONTEXT.Draw(_vertices->GetCount(), 0);
  }
  else
  {
    _indexes->SetAsSource();
    DX_CONTEXT.DrawIndexed(_indexes->GetCount(), 0, 0);
  }
}

//if (_pShader && !_material)
//_material = new Material();

/*void Mesh::SetMaterial(Material* material)
{
  _material = material;
}*/

void Mesh::updateParameters()
{
  Element::updateParameters();
  /*if (_material)
  {
    if (_pShader == NULL)
      _deviceDX.SetMaterial(&_material->GetAsD3DMaterial());
  }
*/
}

void Mesh::returnParameters()
{
  /*for(UINT i = 0; i < _textures.size(); i++)
      _deviceDX.SetTexture(i, NULL);
  */
}

void Mesh::RemoveTexture(const wchar_t* path)
{
  for (TextureList::iterator i = _textures.begin(); i != _textures.end(); ++i)
  {
    if (!wcscmp((*i)->GetPath(), path))
    {
      DX.RemoveTexture((*i)->GetPath());
      _textures.erase(i);
    }
  }
}

void Mesh::SetTexture(const wchar_t* path)
{
  for (UINT i = 0; i < _textures.size(); i++)
    if (!wcscmp(_textures[i]->GetPath(), path))
      return;

  _textures.push_back(DX.GetTexture(path));
}

void Mesh::dirtyTextures()
{
  for (UINT i = 0; i < _textures.size(); i++)
    DX.RemoveTexture(_textures[i]);
  _textures.clear();
}
