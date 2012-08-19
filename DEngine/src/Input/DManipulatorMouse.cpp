#include <Input/DManipulatorMouse.h>

using namespace dev;

ManipulatorMouse::ManipulatorMouse(std::wstring name) :
  Manipulator(name)
{
}

ManipulatorMouse::~ManipulatorMouse()
{
}

void ManipulatorMouse::Handle(const InputStruct& is, double deltaTime)
{
  if (!_camera)
    return;

  if (fabs(is.mouseAxis.z) > FLT_EPSILON)
  {
    Vec3 move = _camera->GetDirection() * _camera->GetLengthLook() * 0.1f * is.mouseAxis.z;
    float length = (_camera->GetLook() - _camera->GetPosition() + move).Length();
    if ((length > _camera->GetZNearProjection() || is.mouseAxis.z < 0) &&
        (length < _camera->GetZFarProjection() || is.mouseAxis.z > 0))
      _camera->SetMove(move);
  }
  else if (is.mouseKey[0])
  {
    float dx = is.mouseAxis.x * _sens * 10;
    float dy = is.mouseAxis.y * _sens * 10;

    if (fabs(dx) > FLT_EPSILON || fabs(dy) > FLT_EPSILON)
      _camera->SetMove(_camera->GetXLocal() * -dx + _camera->GetYLocal() * dy);
  }
  else if (is.mouseKey[1])
  {
    float dx = ToRadian(is.mouseAxis.x) * _sens;
    float dy = ToRadian(is.mouseAxis.y) * _sens;
    
    if (fabs(dx) > FLT_EPSILON || fabs(dy) > FLT_EPSILON)
      _camera->ChangeDirection(dx, dy, 0);
  }
  else if (is.mouseKey[2])
  {
    float dx = is.mouseAxis.x * _speed * 25;
    float dy = is.mouseAxis.y * _speed * 25;

    if (fabs(dx) > FLT_EPSILON || fabs(dy) > FLT_EPSILON)
      _camera->SetMoveAndLook(_camera->GetZLocal() * -dy + _camera->GetXLocal() * dx);
  }
}
