#include <Element/DElement.h>

#include <DX/DConstBuffers.h>
#include <DX/DirectX.h>
#include <Element/DScene.h>

using namespace dev;

Element::Element(const Vec3& position, const Vec3& rotation, const Vec3& scale) :
  _parent(NULL),
  _scene(FALSE),
  _type(Buffer::BT_NONE)
{
  SetVisible(TRUE);
  SetPosition(position);
  SetRotation(rotation);
  SetScale(scale);
}

Element::~Element()
{
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

void Element::setParent(Element* parent)
{
  if (_parent == parent)
    return;

  _parent = parent;
  
  if(_parent)
    setScene(_parent->_scene);

  updateParent(UPDATE_BUFFER_TYPE);
}

void Element::clearParent()
{
  if (!_parent)
    return;

  _parent = NULL;

  updateParent(UPDATE_BUFFER_TYPE);
}

void Element::SetVertexShader(UINT passNum, const wchar_t* path, VertexShader::TypeVertexShader type, const char* funcName)
{
  setVertexShader(passNum, DX.GetVertexShader(path, type, funcName));
}

void Element::SetVertexShader(UINT passNum, VertexShader* vs)
{
  DX.RegistrationVertexShader(vs);
  setVertexShader(passNum, vs);
}

void Element::setVertexShader(UINT passNum, VertexShader* vs)
{
  if (_shaderPasses.find(passNum) == _shaderPasses.end())
    _shaderPasses[passNum] = ShaderStruct();

  ShaderStruct& ss = _shaderPasses[passNum];

  if (ss.vs != vs)
  {
    if(_scene)
      _scene->removeElementShaderPass(this, passNum, ss);

    ss.vs = vs;

    if (_type != Buffer::BT_NONE && ss.vs->GetBufferType() == Buffer::BT_NONE)
      ss.vs->CreateLayout(_type);

    updateParent(UPDATE_BUFFER_TYPE);

    if (_scene)
      _scene->addElementShaderPass(this, passNum, ss);
  }
}

void Element::SetPixelShader(UINT passNum, const wchar_t* path, PixelShader::TypePixelShader type, const char* funcName)
{
  setPixelShader(passNum, DX.GetPixelShader(path, type, funcName));
}

void Element::SetPixelShader(UINT passNum, PixelShader* ps)
{
  DX.RegistrationPixelShader(ps);
  setPixelShader(passNum, ps);
}

void Element::setPixelShader(UINT passNum, PixelShader* ps)
{
  if (_shaderPasses.find(passNum) == _shaderPasses.end())
    _shaderPasses[passNum] = ShaderStruct();

  ShaderStruct& ss = _shaderPasses[passNum];
  if(ss.ps != ps)
  {
    if(_scene)
      _scene->removeElementShaderPass(this, passNum, ss);
    
    ss.ps = ps;

    if (_scene)
      _scene->addElementShaderPass(this, passNum, ss);
  }
}

void Element::SetShaders(UINT passNum, VertexShader* vs, PixelShader* ps)
{
  SetVertexShader(passNum, vs);
  SetPixelShader(passNum, ps);
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

  if (_type == Buffer::BT_NONE)
    return;

  for(ShaderPassMap::iterator i = _shaderPasses.begin(); i != _shaderPasses.end(); ++i)
  {
    if ((*i).second.vs->GetBufferType() == Buffer::BT_NONE)
      (*i).second.vs->CreateLayout(_type);
  }
}

void Element::draw()
{
}

void Element::updateParameters()
{
  WORLD_BUFFER.UpdateResource(&_matrix);
}

void Element::returnParameters()
{
}

void Element::updateParent(Message msg)
{
  if (_parent)
    _parent->updateParent(msg);
}

void Element::setScene(Scene* scene)
{
  if (scene)
  {
    for (ShaderPassMap::iterator i = _shaderPasses.begin(); i != _shaderPasses.end(); ++i)
      scene->addElementShaderPass(this, (*i).first, (*i).second);
  }
  
  if (_scene)
  {
    for (ShaderPassMap::iterator i = _shaderPasses.begin(); i != _shaderPasses.end(); ++i)
      _scene->removeElementShaderPass(this, (*i).first, (*i).second);
  }

  _scene = scene;
}
