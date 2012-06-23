#ifndef DEV_OUTPUT_CLASS_H
#define DEV_OUTPUT_CLASS_H

#include <Input/DInput.h>

namespace dev
{
  class OutputClass
  {
  public:
    void SetActive(bool active) { _active = active; }
    inline bool GetActive() const { return _active; }

    virtual void Handle(const InputStruct& is, double deltaTime) = 0;

  protected:
    bool _active;
  };
}

#endif