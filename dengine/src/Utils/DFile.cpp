#include <Utils/DFile.h>

#include <Core/DLog.h>

using namespace dev;

#define MAX_BUFFER_SIZE 128*1024*1024

File::File(const wchar_t* path) :
  _path(path),
  _buffer(NULL),
  _sizeBuffer(0),
  _currentReadPointer(0),
  _currentWritePointer(0),
  _isOpened(FALSE)
{
  HANDLE file = CreateFileW(path, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
  if (file == INVALID_HANDLE_VALUE)
  {
    std::wstring buffer(L"File not loaded: ");
    buffer += path;
    WRITE_LOG(buffer.c_str());
    _path.clear();
  }
  else
  {
    DWORD fileSize = GetFileSize(file, 0);

    if (fileSize <= MAX_BUFFER_SIZE)
    {
      _isOpened = TRUE;
      _sizeBuffer = fileSize;

      _buffer = new byte[_sizeBuffer];

      DWORD RWValue = 0;
      ReadFile(file, _buffer, _sizeBuffer, &RWValue, 0);
      checkError(RWValue);

      CloseHandle(file);
    }
  }
}

File::~File()
{
  HANDLE file = CreateFileW(_path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

  if (file != INVALID_HANDLE_VALUE)
  {
    DWORD RWValue = 0;
    WriteFile(file, _buffer, _sizeBuffer, &RWValue, 0);
    SetEndOfFile(file);
    checkError(RWValue);
    CloseHandle(file);
  }

  if (_buffer)
  {
    delete[] _buffer;
    _sizeBuffer = 0;
    _buffer = NULL;
  }
}

void File::WriteInfo(LPVOID info, DWORD size)
{
  if (size <= 0)
    return;
  
  if (_currentWritePointer + size >= _sizeBuffer)
    size = _sizeBuffer - _currentWritePointer;

  memcpy(&_buffer[_currentWritePointer], info, size);
  _currentWritePointer += size;
}

void File::RewriteInfo(const DWORD& infoPosition, DWORD size)
{
  if (size <= 0 || _currentWritePointer == infoPosition || infoPosition >= _sizeBuffer)
  {
    _currentWritePointer += size;
    return;
  }

  if (_currentWritePointer + size >= _sizeBuffer)
    size = _sizeBuffer - _currentWritePointer;

  if (infoPosition + size >= _sizeBuffer)
    size = _sizeBuffer - infoPosition;

  memcpy(&_buffer[_currentWritePointer], &_buffer[infoPosition], size);
  _currentWritePointer += size;
}

void File::WriteEndFile()
{
  _sizeBuffer = _currentWritePointer;
  _currentWritePointer += sizeof(DWORD);
}

byte* File::ReadPointerInfo(DWORD position, DWORD size)
{
  if (size == 0 || size > _sizeBuffer)
    return NULL;

  return &_buffer[position];
}

byte* File::ReadCurrentInfo(DWORD size)
{
  byte* point = ReadPointerInfo(_currentReadPointer, size);
  if (point)
    _currentReadPointer += size;
  return point;
}

void File::checkError(DWORD& num)
{
  if (_sizeBuffer != num)
  {
    WRITE_FILE_LOG(L"Error write or read file: ", _path);
  }
}
