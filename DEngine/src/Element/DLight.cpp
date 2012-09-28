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

//spot light
PointLight::PointLight(const Vec3& position, const Vec3& color, const float& radius) : 
  Light(position, color, radius)
{
}

PointLight::~PointLight()
{
}

//direction light
DirectionLight::DirectionLight(const Vec3& direction, const Vec3& position, const Vec3& color, const float& radius) : 
  Light(position, color, radius)
{
  SetDirection(direction);
}

DirectionLight::~DirectionLight()
{
}

void DirectionLight::SetDirection(const Vec3& value)
{
  _direction = value;
}

//conus light
SpotLight::SpotLight(const float& angle, const Vec3& direction, const Vec3& position, const Vec3& color, const float& radius) : 
  DirectionLight(direction, position, color, radius)
{
  SetAngle(angle);
}

SpotLight::~SpotLight()
{
}

void SpotLight::SetAngle(const float& value)
{
  _angle = value;
}
