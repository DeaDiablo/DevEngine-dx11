#ifndef DEV_INPUT_H
#define DEV_INPUT_H

#include <Windows.h>
#include <Input/DScanCodes.h>

namespace dev
{
  struct InputStruct
  {
    byte mouseKey[5];
    struct Axis
    {
      float x, y, z;
    } mouseAxis;
    bool key[256];

    bool GetKeyPressed(DWORD SC_key) const
    {
      return key[SC_key];
    }
  };

  class Input
  {
  public:
    Input(bool mouse = true, bool keyboard = true);
    virtual ~Input();

    void OnInput(bool mouse, bool keyboard);
    void OffInput();

    //mouse
    bool GetClickMouseButton(int button) const;
    float GetMouseDeltaX() const;
    float GetMouseDeltaY() const;
    float GetMouseDeltaZ() const;

    //keyboard
    bool GetKeyPressed(byte SC_KEY) const;
    
    InputStruct GetInputStruct() const;

  protected:
    bool      _mouse;
    bool      _keyboard;
    HHOOK     _hook;
  };
}

#endif