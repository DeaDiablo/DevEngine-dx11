#include <Element/DElement.h>

#include <DX/DConstBuffers.h>
#include <DX/DirectX.h>

using namespace dev;

Element::Element(const Vec3& position, const Vec3& rotation, const Vec3& scale) :
  _parent(NULL),
  _type(Buffer::BT_NONE)
{
  SetVisible(TRUE);
  SetPosition(position);
  SetRotation(rotation);
  SetScale(scale);
  _wBuffer = Buffer::WorldConstBuffer::Register();
}

Element::~Element()
{
  Buffer::WorldConstBuffer::Unregister();
  for(ShaderPassMap::iterator i = _shaderPasses.begin(); i != _shaderPasses.end(); ++i)
    delete (*i).second;
}

void Element::SetVisible(bool value)
{
  _visible = value;
}

void Element::SetMove(const float& x, const float& y, const float& z)
{
  _position += Vec3(x, y, z);
}

void Element::SetMove(const Vec3& value)
{
  _position += value;
}

void Element::SetPosition(const float& x, const float& y, const float& z)
{
  _position = Vec3(x, y, z);
}

void Element::SetPosition(const Vec3& value)
{
  _position = value;
}

void Element::SetScale(const float& sx, const float& sy, const float& sz)
{
  _scale = Vec3(sx, sy, sz);
}

void Element::SetScale(const Vec3& value)
{
  _scale = value;
}

void Element::SetRotation(const float& angleX, const float& angleY, const float& angleZ)
{
  _rotation = Vec3(angleX, angleY, angleZ);
}

void Element::SetRotation(const Vec3& value)
{
  _rotation = value;
}

void Element::SetParent(Element* parent)
{
  _parent = parent;

  updateParent(UPDATE_BUFFER_TYPE);
}

void Element::ClearParent()
{
  _parent = NULL;

  updateParent(UPDATE_BUFFER_TYPE);
}

void Element::SetVertexShader(UINT passNum, const wchar_t* path, VertexShader::TypeVertexShader type, const char* funcName)
{
  if (_shaderPasses.find(passNum) == _shaderPasses.end())
    _shaderPasses[passNum] = new ShaderPass(passNum);
  
  _shaderPasses[passNum]->SetVertexShader(path, type, funcName);
  
  if (_type != Buffer::BT_NONE)
    _shaderPasses[passNum]->SetBufferType(_type);

  updateParent(UPDATE_BUFFER_TYPE);
}

void Element::SetVertexShader(UINT passNum, VertexShader* vs)
{
  if (_shaderPasses.find(passNum) == _shaderPasses.end())
    _shaderPasses[passNum] = new ShaderPass(passNum);

  _shaderPasses[passNum]->SetVertexShader(vs);

  if (_type != Buffer::BT_NONE)
    _shaderPasses[passNum]->SetBufferType(_type);

  updateParent(UPDATE_BUFFER_TYPE);
}

void Element::SetPixelShader(UINT passNum, const wchar_t* path, PixelShader::TypePixelShader type, const char* funcName)
{
  if (_shaderPasses.find(passNum) == _shaderPasses.end())
    _shaderPasses[passNum] = new ShaderPass(passNum);
  
  _shaderPasses[passNum]->SetPixelShader(path, type, funcName);
}

void Element::SetPixelShader(UINT passNum, PixelShader* ps)
{
  if (_shaderPasses.find(passNum) == _shaderPasses.end())
    _shaderPasses[passNum] = new ShaderPass(passNum);

  _shaderPasses[passNum]->SetPixelShader(ps);
}

void Element::Update(const Matrix& matrix)
{
  if (!_visible())
    return;

  _matrix.MakeTransform(_position(), _rotation(), _scale());
  _matrix *= matrix;
}

void Element::DrawElement()
{
  if (!_visible())
    return;

  updateParameters();
  draw();
  returnParameters();
}

void Element::setBufferType(Buffer::BufferType BT_Type)
{
  if (_type == BT_Type)
    return;

  _type = BT_Type;
  
  for(ShaderPassMap::iterator i = _shaderPasses.begin(); i != _shaderPasses.end(); ++i)
    (*i).second->SetBufferType(BT_Type);
}

void Element::draw()
{
}

void Element::updateParameters()
{
  _wBuffer->UpdateResource(&_matrix);
}

void Element::returnParameters()
{
}

void Element::updateParent(Message msg)
{
  if (_parent)
    _parent->updateParent(msg);
}
