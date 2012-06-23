#include <Input/DManipulatorWASD.h>

using namespace dev;

ManipulatorWASD::ManipulatorWASD(std::wstring name) :
  Manipulator(name),
  _mouseX(0),
  _mouseY(0)
{
}

ManipulatorWASD::~ManipulatorWASD()
{
}

void ManipulatorWASD::Handle(const InputStruct& is, double deltaTime)
{
  if (!_camera)
    return;

  float dx = ToRadian(is.mouseAxis.x);
  float dy = ToRadian(is.mouseAxis.y);
  _mouseX += dx * _sens;
  _mouseY += dy * _sens;
  if (_mouseY > PI_2 - Degree * 0.1f)
    _mouseY = PI_2 - Degree * 0.1f;
  if (_mouseY < -PI_2 + Degree *  0.1f)
    _mouseY = -PI_2 + Degree *  0.1f;
  float mouseZ = ToRadian(is.mouseAxis.z);

  if (fabs(dx) > FLT_EPSILON || fabs(dy) > FLT_EPSILON)
    _camera->SetDirection(dev::Matrix::Rotate(_mouseY, _mouseX, 0) * dev::Vec3(0, 0, 1));

  Vec3 vec(0,0,0);
  if (is.GetKeyPressed(SC_W))
    vec += _camera->GetDirection();
  if (is.GetKeyPressed(SC_S))
    vec -= _camera->GetDirection();
  if (is.GetKeyPressed(SC_D))
    vec += _camera->GetRight();
  if (is.GetKeyPressed(SC_A))
    vec -= _camera->GetRight();
  if (is.GetKeyPressed(SC_Q))
    vec += _camera->GetUp();
  if (is.GetKeyPressed(SC_Z))
    vec -= _camera->GetUp();

  vec.Normalize();
  if (vec != dev::Vec3(0, 0, 0))
    _camera->SetMove(vec * _speed * (float)deltaTime);
}
