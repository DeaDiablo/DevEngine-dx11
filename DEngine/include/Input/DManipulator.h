#ifndef DEV_MANIPULATOR_H
#define DEV_MANIPULATOR_H

#include <string>
#include <Element/DCamera.h>
#include <Input/DOutputClass.h>

namespace dev
{
  class Camera;

  class Manipulator :
    public OutputClass
  {
  public:
    Manipulator(std::wstring name);
    virtual ~Manipulator();

    void SetCamera(Camera* camera);
    inline Camera* GetCamera() const
    {
      return _camera;
    }

    void SetSens(const float& sens);
    inline float GetSens() const
    {
      return _sens;
    }

    void SetSpeed(const float& speed);
    inline float GetSpeed() const
    {
      return _speed;
    }

    virtual void Handle(const InputStruct& is, double deltaTime) = 0;

  protected:
    float _sens,  _speed;
    std::wstring _name;
    Camera* _camera;
  };
}

#endif