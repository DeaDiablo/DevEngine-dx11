#include <Core/DTimer.h>

using namespace dev;

Timer::Timer() :
  _numProc(1),
  _thread(0)
{
  init();
}

Timer::~Timer()
{
}

Timer* Timer::GetTimer()
{
  static Timer timer;
  return &timer;
}

void Timer::init()
{
  DWORD_PTR procMask;
  DWORD_PTR sysMask;
  GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

  if (procMask == 0)
     procMask = 1;

  _numProc = 1;
  while((procMask & _numProc) == 0)
  {
    procMask <<= 1;
  }

  _numProc = procMask;
 
  _thread = GetCurrentThread();
 
  DWORD_PTR oldMask = SetThreadAffinityMask(_thread, _numProc);

  LARGE_INTEGER _frequency;

  if (QueryPerformanceFrequency(&_frequency) &&
      QueryPerformanceCounter(&_counter))
  {
    _freq         = (double) 1 / _frequency.QuadPart;
    _startTime[0] = (double)_counter.QuadPart;
    
    for(byte i = 1; i < 255; i++)
    {
      _startTime[i] = _startTime[0];
    }

    memcpy(_tickTime,      _startTime, 256 * sizeof(double));
    memcpy(_prevTickTime,  _tickTime,  256 * sizeof(double));
    memcpy(_deltaTime,     _startTime, 256 * sizeof(double));
    memcpy(_prevDeltaTime, _deltaTime, 256 * sizeof(double));
  }
  else
  {
    _freq = 0;
  }

  SetThreadAffinityMask(_thread, oldMask);
}

void Timer::Reset(byte numTimer)
{
  DWORD_PTR oldMask = SetThreadAffinityMask(_thread, _numProc);
  QueryPerformanceCounter(&_counter);
  SetThreadAffinityMask(_thread, oldMask);

  _startTime[numTimer]      = (double)_counter.QuadPart;
  
  _tickTime[numTimer]       = getTime(numTimer);
  _prevTickTime[numTimer]   = _tickTime[numTimer];
  _deltaTime[numTimer]      = _tickTime[numTimer];
  _prevDeltaTime[numTimer]  = _tickTime[numTimer];
}

double Timer::GetTimeNS(byte numTimer)
{
  return getTime(numTimer) * 1e9;
}

double Timer::GetTimeMkS(byte numTimer)
{
  return getTime(numTimer) * 1e6;
}

double Timer::GetTimeMS(byte numTimer)
{
  return getTime(numTimer) * 1e3;
}

double Timer::GetTimeS(byte numTimer)
{
  return getTime(numTimer);
}

double Timer::GetDeltaTimeNS(byte numTimer)
{
  if (!_freq)
    return 0;

  _prevTickTime[numTimer] = _tickTime[numTimer];
  _tickTime[numTimer] = getTime(numTimer) * 1e9;

  return (_tickTime[numTimer] - _prevTickTime[numTimer]);
}

double Timer::GetDeltaTimeMkS(byte numTimer)
{
  if (!_freq)
    return 0;

  _prevTickTime[numTimer] = _tickTime[numTimer];
  _tickTime[numTimer] = getTime(numTimer) * 1e6;

  return (_tickTime[numTimer] - _prevTickTime[numTimer]);
}

double Timer::GetDeltaTimeMS(byte numTimer)
{
  if (!_freq)
    return 0;

  _prevTickTime[numTimer] = _tickTime[numTimer];
  _tickTime[numTimer] = getTime(numTimer) * 1e3;

  return (_tickTime[numTimer] - _prevTickTime[numTimer]);
}

double Timer::GetDeltaTimeS(byte numTimer)
{
  if (!_freq)
    return 0;

  _prevTickTime[numTimer] = _tickTime[numTimer];
  _tickTime[numTimer] = getTime(numTimer);

  return (_tickTime[numTimer] - _prevTickTime[numTimer]);
}

bool Timer::GetEventTimeNS(byte numTimer, double factor)
{
  if (!_freq)
    return 0;

  _deltaTime[numTimer] = getTime(numTimer) * 1e9;

  if (_deltaTime[numTimer] - _prevDeltaTime[numTimer] < factor)
    return false;

  _prevDeltaTime[numTimer] = _prevDeltaTime[numTimer] + factor;

  return true;
}

bool Timer::GetEventTimeMkS(byte numTimer, double factor)
{
  if (!_freq)
    return 0;

  _deltaTime[numTimer] = getTime(numTimer) * 1e6;

  if (_deltaTime[numTimer] - _prevDeltaTime[numTimer] < factor)
    return false;

  _prevDeltaTime[numTimer] = _prevDeltaTime[numTimer] + factor;

  return true;
}

bool Timer::GetEventTimeMS(byte numTimer, double factor)
{
  if (!_freq)
    return 0;

  _deltaTime[numTimer] = getTime(numTimer) * 1e3;

  if (_deltaTime[numTimer] - _prevDeltaTime[numTimer] < factor)
    return false;

  _prevDeltaTime[numTimer] = _prevDeltaTime[numTimer] + factor;

  return true;
}

bool Timer::GetEventTimeS(byte numTimer, double factor)
{
  if (!_freq)
    return 0;

  _deltaTime[numTimer] = getTime(numTimer);

  if (_deltaTime[numTimer] - _prevDeltaTime[numTimer] < factor)
    return false;

  _prevDeltaTime[numTimer] = _prevDeltaTime[numTimer] + factor;

  return true;
}

double Timer::getTime(byte numTimer)
{
  if (!_freq)
    return 0;

  DWORD_PTR oldMask = SetThreadAffinityMask(_thread, _numProc);
  QueryPerformanceCounter(&_counter);
  SetThreadAffinityMask(_thread, oldMask);

  return double(_counter.QuadPart - _startTime[numTimer]) * _freq;
}
