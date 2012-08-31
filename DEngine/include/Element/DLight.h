#ifndef DEV_LIGHT_H
#define DEV_LIGHT_H

#include <Element/DProperty.h>
#include <Math/DVec3.h>

namespace dev
{
  class Light
  {
  public:
    Light(const Vec3& position = Vec3(0.0f, 0.0f, 0.0f), const Vec3& color = Vec3(255.0f, 255.0f, 255.0f), const float& radius = float(100.0f) );
    virtual ~Light();

    void SetPosition(const float& x, const float& y, const float& z);
    void SetPosition(const Vec3& value);
    inline const Vec3& GetPosition() const
    {
      return _position();
    }

    void SetColor(const float& r, const float& g, const float& b);
    void SetColor(const Vec3& value);
    inline const Vec3& GetColor() const
    {
      return _color();
    }

    void SetRadius(const float& value);
    inline const float& GetRadius() const
    {
      return _radius();
    }

  protected:
    Properties::Property<Vec3>  _position;
    Properties::Property<Vec3>  _color;
    Properties::Property<float> _radius;
  };
}

#endif