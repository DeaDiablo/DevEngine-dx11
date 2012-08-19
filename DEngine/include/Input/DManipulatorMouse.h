#ifndef DEV_MANIPULATOR_MOUSE_H
#define DEV_MANIPULATOR_MOUSE_H

#include <Input/DManipulator.h>

namespace dev
{
  class ManipulatorMouse :
    public Manipulator
  {
  public:
    ManipulatorMouse(std::wstring name);
    virtual ~ManipulatorMouse();

    virtual void Handle(const InputStruct& is, double deltaTime);
  };
}

#endif