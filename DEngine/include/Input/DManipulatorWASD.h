#ifndef DEV_MANIPULATOR_WASD_H
#define DEV_MANIPULATOR_WASD_H

#include <Input/DManipulator.h>

namespace dev
{
  class ManipulatorWASD :
    public Manipulator
  {
  public:
    ManipulatorWASD(std::wstring name);
    virtual ~ManipulatorWASD();

    virtual void Handle(const InputStruct& is, double deltaTime);

  protected:
    float _mouseX, _mouseY;
  };
}

#endif