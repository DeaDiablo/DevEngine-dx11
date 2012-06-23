#ifndef DEV_SYNC_H
#define DEV_SYNC_H

#include <Windows.h>

namespace dev
{
  class CriticalSection
  {
  public:
    CriticalSection();
    virtual ~CriticalSection();

    void Lock();
    bool TryLock();
    void UnLock();

  protected:
    CRITICAL_SECTION _cs;
  };

  class Mutex
  {
  public:
    Mutex(const wchar_t* nameMutex);
    virtual ~Mutex();

    bool Lock();
    bool TryLock();
    void UnLock();

    inline const wchar_t* GetName()
    {
      return _name;
    }

  protected:
    HANDLE _mutex;
    const wchar_t* _name;
  };

  class Event
  {
  public:
    Event(const wchar_t* nameEvent);
    virtual ~Event();

    BOOL Set();
    BOOL Reset();
    bool Wait(DWORD time);

  protected:
    HANDLE _event;
  };

  class Semaphore
  {
  public:
    Semaphore(long numberRequests, const wchar_t* nameSemaphore);
    virtual ~Semaphore();

    bool Lock();
    bool TryLock();
    void UnLock();

    inline const wchar_t* GetName()
    {
      return _name;
    }

  protected:
    HANDLE _semaphore;
    const wchar_t* _name;
  };
}

#endif