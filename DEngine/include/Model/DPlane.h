#ifndef DEV_PLANE_H
#define DEV_PLANE_H

#include <Element/DCamera.h>
#include <Model/DMesh.h>

namespace dev
{
  class Plane : public Mesh
  {
  public:
    Plane(const wchar_t* name, const Vec2& leftTop, const Vec2& rightBottom);
    virtual ~Plane();

    void SetPlain(const Vec2& leftTop, const Vec2& rightBottom);
  };

  class PlaneIdentity : public Plane
  {
  public:
    PlaneIdentity(const wchar_t* name);
    virtual ~PlaneIdentity();
  };
}

#endif