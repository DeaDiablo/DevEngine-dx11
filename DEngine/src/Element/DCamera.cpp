#include <Element/DCamera.h>

#include <DX/DirectX.h>
#include <Core/DSettings.h>

using namespace dev;

Camera::Camera() :
  _updateViewOrProjection(true)
{
  SetPosition(Vec3(0, 10, -50));
  SetLook(Vec3(0, 0, 0));
  SetUp(Vec3(0, 1, 0));
  SetNewProjection(PI_4, (float)DX.GetWidth()/DX.GetHeight(), DEFAULT_NEAR, DEFAULT_FAR);
  init();
}

Camera::Camera(Vec3 position, Vec3 look, Vec3 up) :
  _updateViewOrProjection(true)
{
  SetPosition(position);
  SetLook(look);
  SetUp(up);
  SetNewProjection(PI_4, (float)DX.GetWidth()/DX.GetHeight(), DEFAULT_NEAR, DEFAULT_FAR);
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
}

void Camera::init()
{
}

void Camera::SetMove(const float& x, const float& y, const float& z)
{
  SetMove(Vec3(x, y, z));
}

void Camera::SetMove(const Vec3& value)
{
  _position = _position() + value;
  updateDirectionAndRight();
  _updateViewOrProjection = true;
}

void Camera::SetMoveAndLook(const float& x, const float& y, const float& z)
{
  SetPosition(_position() + Vec3(x, y, z));
}

void Camera::SetMoveAndLook(const Vec3& value)
{
  SetPosition(_position() + value);
}

void Camera::SetPosition(const float& x, const float& y, const float& z)
{
  SetPosition(Vec3(x, y, z));
}

void Camera::SetPosition(const Vec3& value)
{
  _look = value + _direction * _lengthLook;
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
  _yLocal = buf;
  updateDirectionAndRight();
  _updateViewOrProjection = true;
}

void Camera::SetDirection(const float& x, const float& y, const float& z)
{
  SetDirection(Vec3(x, y, z));
}

void Camera::SetDirection(const Vec3& value)
{
  if(value * _zLocal > 0)
    _direction = value;
  else if (value * _zLocal < 0)
    _direction = Vec3(-value.x * 0.001f, value.y, -value.z * 0.001f);
  else
    return;

  _direction.Normalize();
  _look = _position() + _direction * _lengthLook;

  _zLocal = Vec3(_direction.x, 0, _direction.z);
  _zLocal.Normalize();

  _xLocal = _yLocal ^ _zLocal;
  _xLocal.Normalize();

  _updateViewOrProjection = true;
}

void Camera::ChangeDirection(const float& yaw, const float& pitch, const float& roll)
{
  ChangeDirection(Vec3(yaw, pitch, roll));
}

void Camera::ChangeDirection(const Vec3& value)
{
  Matrix yprMatrix =  Matrix::Rotate(_zLocal, value.z) * Matrix::Rotate(_yLocal, value.x) * Matrix::Rotate(_xLocal, value.y);
  SetDirection(yprMatrix * _direction);
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
    _updateViewOrProjection = false;
    Vec3 pos = _position();
    _view = Matrix::Look(pos, _look(), _up());
    _view = _view * _projection;
    VIEW_PROJECTION_BUFFER.UpdateResource(&_view);
    Vec4 eyePos(pos.x, pos.y, pos.z, 1.0f);
    EYE_POSITION_BUFFER.UpdateResource(&eyePos);
  }
}

void Camera::updateDirectionAndRight()
{
  _direction = (_look() - _position());
  _lengthLook = _direction.Length();
  _direction.Normalize();
  
  _zLocal = Vec3(_direction.x, 0, _direction.z);
  _zLocal.Normalize();
  
  _xLocal = _yLocal ^ _zLocal;
  _xLocal.Normalize();
}

void Camera::SetManipulator(Manipulator* manipulator)
{
  _manipulator = manipulator;
  _manipulator->SetCamera(this);
}
