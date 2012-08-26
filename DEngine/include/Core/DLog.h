#ifndef DEV_LOG_H
#define DEV_LOG_H

#include <Core/DSettings.h>
#include <Core/DSystemLib.h>
#include <stdio.h>

namespace dev
{
  class Log
  {
  public:
    static Log* GetLog()
    {
      static Log* log = new Log(logFilename);
      return log;
    }

    void WriteToLog(const wchar_t* textToLog)
    {
      if (_fileLog != INVALID_HANDLE_VALUE)
      {
        DWORD writedBytes;
        int secs = (int)SYS_TIMER.GetTimeS(0);
        int mins = secs / 60;
        int hours = mins / 60;
        secs -= mins * 60;
        mins -= hours * 60;
        wchar_t time[255];
        swprintf_s(time, 255, L"[%i:%02i:%02i] : ", hours, mins, secs);
        WriteFile(_fileLog, time, (DWORD)wcslen(time) * sizeof(wchar_t), &writedBytes, 0);
        WriteFile(_fileLog, textToLog, (DWORD)wcslen(textToLog) * sizeof(wchar_t), &writedBytes, 0);
        WriteFile(_fileLog, L"\r\n", 2 * sizeof(wchar_t), &writedBytes, 0);
      }
    }

  protected:
    Log(const wchar_t* filename)
    {
      _fileLog = CreateFileW(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
      SYS_TIMER.Reset(0);
    }
    virtual ~Log()
    {
      if (_fileLog)
        CloseHandle(_fileLog);
    }

    HANDLE _fileLog;
  };
}

#endif