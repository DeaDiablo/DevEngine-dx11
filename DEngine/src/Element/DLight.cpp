#include <Element/DLight.h>

using namespace dev;

Light::Light(const Vec3& position, const Vec3& color, const float& radius)
{
  SetPosition(position);
  SetColor(color);
  SetRadius(radius);
}

Light::~Light()
{

}

void Light::SetPosition(const float& x, const float& y, const float& z)
{
  _position = Vec3(x, y, z);
}

void Light::SetPosition(const Vec3& value)
{
  _position = value;
}

void Light::SetColor(const float& r, const float& g, const float& b)
{
  _color = Vec3(r, g, b);
}

void Light::SetColor(const Vec3& value)
{
  _color = value;
}

void Light::SetRadius(const float& value)
{
  _radius = value;
}
