#include <Element/DElement.h>

#include <DX/DConstBuffers.h>
#include <DX/DirectX.h>

using namespace dev;

Element::Element(const Vec3& position, const Vec3& rotation, const Vec3& scale) :
  _vShader(NULL),
  _pShader(NULL),
  _layout(NULL),
  _parent(NULL),
  _type(Buffer::BT_NONE)
{
  SetVisible(true);
  SetPosition(position);
  SetRotation(rotation);
  SetScale(scale);
  _wBuffer = Buffer::WorldConstBuffer::Register();
}

Element::~Element()
{
  if (_vShader)
  {
    DX->RemoveVertexShader(_vShader);
    _vShader = NULL;
  }

  if (_layout)
  {
    DX->RemoveLayout(_layout);
    _layout = NULL;
  }

  if (_pShader)
  {
    DX->RemovePixelShader(_pShader);
    _pShader = NULL;
  }

  Buffer::WorldConstBuffer::Unregister();
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
}

void Element::ClearParent()
{
  _parent = NULL;
}

void Element::createLayout()
{
  if (_layout)
    DX->RemoveLayout(_layout);

  if (_type != Buffer::BT_NONE && _type < Buffer::VB_POINTER && _vShader)
    _layout = DX->GetLayout(_type, _vShader);
}

void Element::SetVertexShader(const wchar_t* path, VertexShader::TypeVertexShader type, const char* funcName)
{
  if (_vShader)
    DX->RemoveVertexShader(_vShader);

  _vShader = DX->GetVertexShader(path, type, funcName);

  createLayout();
}

void Element::SetLayout(Buffer::BufferType BT_Type)
{
  _type = BT_Type;
  Element::createLayout();
}

void Element::SetPixelShader(const wchar_t* path, PixelShader::TypePixelShader type, const char* funcName)
{
  if (_pShader)
    DX->RemovePixelShader(_pShader);

  _pShader = DX->GetPixelShader(path, type, funcName);
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