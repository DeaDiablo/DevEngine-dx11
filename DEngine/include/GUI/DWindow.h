#ifndef DEV_WINDOW_H
#define DEV_WINDOW_H

#include <Windows.h>

namespace dev
{
  class Window
  {
  public:
    Window();
    virtual ~Window();

    HWND Create(HINSTANCE hInst, int PosX, int PosY, int Width, int Height);

  private:
    void destroy();

    static LRESULT CALLBACK windowProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK msgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

    HINSTANCE    _hInst;
    HWND         _hWnd;

    int          _width;
    int          _height;

    PAINTSTRUCT _ps;
    HDC         _hdc;
  };
}

#endif