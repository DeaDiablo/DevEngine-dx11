#ifndef DEV_CAMERA_H
#define DEV_CAMERA_H

#include <Math/DMatrix.h>
#include <DX/DConstBuffers.h>
#include <Element/DProperty.h>
#include <Input/DManipulator.h>

namespace dev
{
  class Manipulator;

  class Camera
  {
  public:
    Camera();
    Camera(Vec3 position, Vec3 look, Vec3 up);
    Camera(Vec3 position, Vec3 look, Vec3 up, float fovY, float aspect, float znear, float zfar);
    virtual ~Camera();

    void SetMove(const float& x, const float& y, const float& z);
    void SetMove(const Vec3& value);

    void SetMoveAndLook(const float& x, const float& y, const float& z);
    void SetMoveAndLook(const Vec3& value);

    void SetPosition(const float& x, const float& y, const float& z);
    void SetPosition(const Vec3& value);
    inline const Vec3& GetPosition() const
    {
      return _position();
    }

    void SetLook(const float& x, const float& y, const float& z);
    void SetLook(const Vec3& value);
    inline const Vec3& GetLook() const
    {
      return _look();
    }
    inline const float& GetLengthLook() const
    {
      return _lengthLook;
    }

    void SetUp(const float& x, const float& y, const float& z);
    void SetUp(const Vec3& value);
    inline const Vec3& GetUp() const
    {
      return _up();
    }

    inline const Vec3& GetZLocal() const
    {
      return _zLocal;
    }

    inline const Vec3& GetXLocal() const
    {
      return _xLocal;
    }

    inline const Vec3& GetYLocal() const
    {
      return _yLocal;
    }

    void SetDirection(const float& x, const float& y, const float& z);
    void SetDirection(const Vec3& value);
    inline const Vec3& GetDirection() const 
    {
      return _direction;
    }

    void ChangeDirection(const float& yaw, const float& pitch, const float& roll);
    void ChangeDirection(const Vec3& value);

    void SetNewProjection(float fovY, float aspect, float znear, float zfar);
    inline float GetFovYProjection() const
    {
      return _fovY();
    }

    inline float GetAspectProjection() const
    {
      return _aspect();
    }

    inline float GetZNearProjection() const
    {
      return _zn();
    }

    inline float GetZFarProjection() const
    {
      return _zf();
    }

    inline const Matrix& GetViewMatrix() const
    {
      return _view;
    }

    inline const Matrix& GetProjectionMatrix() const
    {
      return _projection;
    }

    void SetManipulator(Manipulator* manipulator);
    inline Manipulator* GetManipulator() const
    {
      return _manipulator;
    }

    void Update();

  protected:
    void init();
    void updateDirectionAndRight();
    Matrix _view, _projection;
    bool   _updateViewOrProjection;
    Buffer::ConstantBuffer* _vpBuffer;

    Properties::Property<float>  _fovY, _aspect, _zn, _zf;
    Properties::Property<Vec3>   _position;
    Properties::Property<Vec3>   _look;
    Properties::Property<Vec3>   _up;

    Vec3   _direction;
    float  _lengthLook;

    Vec3   _xLocal, _yLocal, _zLocal;

    Manipulator* _manipulator;
  };
}

#endif