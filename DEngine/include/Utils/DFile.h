#ifndef DEV_FILE_H
#define DEV_FILE_H

#include <Core/DInclude.h>

namespace dev
{
  class File
  {
  public:
    File(const wchar_t* path);
    ~File();

    inline const DWORD& GetCurrentReadPointer() const 
    {
      return _currentReadPointer;
    }
    void SetCurrentReadPointer(const DWORD& pointer)
    {
      _currentReadPointer = pointer;
    }

    inline const DWORD& GetCurrentWritePointer() const 
    {
      return _currentWritePointer;
    }
    void SetCurrentWritePointer(const DWORD& pointer)
    {
      _currentWritePointer = pointer;
    }
    void SetCurrentWritePointer()
    {
      _currentWritePointer = _currentReadPointer;
    }

    template <class T>
    void WriteInfo(T& info)
    {
      WriteInfo(&info, sizeof(T));
    }
    void WriteInfo(LPVOID info, DWORD size);
    void RewriteInfo(const DWORD& infoPosition, DWORD size);
    void WriteEndFile();

    byte* ReadPointerInfo(DWORD position, DWORD size);
    byte* ReadCurrentInfo(DWORD size);

    template <class T>
    inline T& ReadPointerInfo(DWORD position)
    {
      return (T&)(*ReadPointerInfo(position, sizeof(T)));
    }

    template <class T>
    inline T& ReadCurrentInfo()
    {
      return (T&)(*ReadCurrentInfo(sizeof(T)));
    }

    inline const wchar_t* GetPath() const
    {
      return _path.c_str();
    }

    inline const bool& IsOpened() const 
    { 
      return _isOpened;
    }
    
    inline const DWORD& GetSize() const
    {
      return _sizeBuffer;
    }

  protected:
    void checkError(DWORD& num);
    std::wstring  _path;
    bool          _isOpened;

    DWORD _currentReadPointer, _currentWritePointer;

    byte* _buffer;
    DWORD _sizeBuffer;
  };
}

#endif