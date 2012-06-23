#include <Input/DManipulator.h>

#include <Core/DSettings.h>

using namespace dev;

Manipulator::Manipulator(std::wstring name) :
  _name(name),
  _camera(NULL),
  _sens(defaultSens),
  _speed(defaultSpeed)
{
}

Manipulator::~Manipulator()
{
  _camera = NULL;
}


void Manipulator::SetCamera(Camera* camera)
{
  _camera = camera;
}

void Manipulator::SetSens(const float& sens)
{
  _sens = sens;
}

void Manipulator::SetSpeed(const float& speed)
{
  _speed = speed;
}

