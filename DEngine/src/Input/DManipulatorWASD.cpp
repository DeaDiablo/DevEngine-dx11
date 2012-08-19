#include <Input/DManipulatorWASD.h>

using namespace dev;

ManipulatorWASD::ManipulatorWASD(std::wstring name) :
  Manipulator(name)
{
}

ManipulatorWASD::~ManipulatorWASD()
{
}

void ManipulatorWASD::Handle(const InputStruct& is, double deltaTime)
{
  if (!_camera)
    return;

  float dx = ToRadian(is.mouseAxis.x) * _sens;
  float dy = ToRadian(is.mouseAxis.y) * _sens;

  if (fabs(dx) > FLT_EPSILON || fabs(dy) > FLT_EPSILON)
    _camera->ChangeDirection(dx, dy, 0);

  Vec3 vec(0,0,0);
  if (is.GetKeyPressed(SC_W))
    vec += _camera->GetDirection();
  if (is.GetKeyPressed(SC_S))
    vec -= _camera->GetDirection();
  if (is.GetKeyPressed(SC_D))
    vec += _camera->GetXLocal();
  if (is.GetKeyPressed(SC_A))
    vec -= _camera->GetXLocal();
  if (is.GetKeyPressed(SC_Q))
    vec += _camera->GetUp();
  if (is.GetKeyPressed(SC_Z))
    vec -= _camera->GetUp();

  vec.Normalize();
  if (vec != dev::Vec3(0, 0, 0))
    _camera->SetMoveAndLook(vec * _speed * (float)deltaTime);
}
