#ifndef DEV_SYSTEM_LIB_H
#define DEV_SYSTEM_LIB_H

#include <Core/DSingleton.h>
#include <Core/DTimer.h>
#include <Core/DSync.h>
#include <Input/DInput.h>

namespace dev
{
  class SystemCS : public Singleton<CriticalSection>
  {};

  class SystemTimer : public Singleton<Timer>
  {};

  class SystemInput : public Singleton<Input>
  {};
}

#endif