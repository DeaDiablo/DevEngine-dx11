#ifndef DEV_DEFERRED_PLANE_H
#define DEV_DEFERRED_PLANE_H

#include <Model/DPlane.h>

namespace dev
{
  class DeferredPlane : public PlaneIdentity
  {
  public:
    DeferredPlane(const wchar_t* name);
    virtual ~DeferredPlane();
  };
}

#endif