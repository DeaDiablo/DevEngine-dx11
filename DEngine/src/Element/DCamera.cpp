#include <Element/DCamera.h>

#include <DX/DirectX.h>

using namespace dev;

Camera::Camera() :
  _updateViewOrProjection(true)
{
  SetPosition(Vec3(0, 10, -50));
  SetLook(Vec3(0, 0, 0));
  SetUp(Vec3(0, 1, 0));
  SetNewProjection(PI_4, (float)DX->GetWidth()/DX->GetHeight(), 0.01f, 1000.0f);
  init();
}

Camera::Camera(Vec3 position, Vec3 look, Vec3 up) :
  _updateViewOrProjection(true)
{
  SetPosition(position);
  SetLook(look);
  SetUp(up);
  SetNewProjection(PI_4, (float)DX->GetWidth()/DX->GetHeight(), 0.01f, 1000.0f);
  init();
}

Camera::Camera(Vec3 position, Vec3 look, Vec3 up, 
               float fovY, float aspect, float znear, float zfar) :
  _updateViewOrProjection(true)
{
  SetPosition(position);
  SetLook(look);
  SetUp(up);
  SetNewProjection(fovY, aspect, znear, zfar);
  init();
}

Camera::~Camera()
{
  Buffer::ViewProjectionConstBuffer::Unregister();
}

void Camera::init()
{
  _vpBuffer = Buffer::ViewProjectionConstBuffer::Register();
}

void Camera::SetMove(const float& x, const float& y, const float& z)
{
  SetPosition(_position() + Vec3(x, y, z));
}

void Camera::SetMove(const Vec3& value)
{
  SetPosition(_position() + value);
}

void Camera::SetPosition(const float& x, const float& y, const float& z)
{
  SetPosition(Vec3(x, y, z));
}

void Camera::SetPosition(const Vec3& value)
{
  _look = value + GetDirection();
  _position = value;
  updateDirectionAndRight();
  _updateViewOrProjection = true;
}

void Camera::SetLook(const float& x, const float& y, const float& z)
{
  SetLook(Vec3(x, y, z));
}

void Camera::SetLook(const Vec3& value)
{
  _look = value;
  updateDirectionAndRight();
  _updateViewOrProjection = true;
}

void Camera::SetUp(const float& x, const float& y, const float& z)
{
  SetUp(Vec3(x, y, z));
}

void Camera::SetUp(const Vec3& value)
{
  Vec3 buf(value);
  buf.Normalize();
  _up = buf;
  updateDirectionAndRight();
  _updateViewOrProjection = true;
}

void Camera::SetDirection(const float& x, const float& y, const float& z)
{
  SetDirection(Vec3(x, y, z));
}

void Camera::SetDirection(const Vec3& value)
{
  _look = _position() + value;
  updateDirectionAndRight();
  _updateViewOrProjection = true;
}

void Camera::SetNewProjection(float fovY, float aspect, float znear, float zfar)
{
  _fovY = fovY;
  _aspect = aspect;
  _zn = znear;
  _zf = zfar;
  _projection = Matrix::Projection(fovY, aspect, znear, zfar);
  _updateViewOrProjection = true;
}

void Camera::Update()
{
  if (_updateViewOrProjection)
  {
    _view = Matrix::Look(GetPosition(), GetLook(), GetUp());
    _view = _view * _projection;
    _vpBuffer->UpdateResource(&_view);
    _updateViewOrProjection = false;
  }
}

void Camera::updateDirectionAndRight()
{
  _direction = (_look() - _position());
  _direction.Normalize();
  _right = _up() ^ _direction;
  _right.Normalize();
}

void Camera::SetManipulator(Manipulator* manipulator)
{
  _manipulator = manipulator;
  _manipulator->SetCamera(this);
}
