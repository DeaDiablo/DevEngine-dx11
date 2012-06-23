#include <GUI/DWindow.h>

#include <Core/DLog.h>

using namespace dev;

Window::Window() :
  _hWnd(NULL),
  _hInst(NULL),
  _width(0),
  _height(0)
{
}

Window::~Window()
{
  destroy();
}

void Window::destroy()
{
  CloseWindow(_hWnd);
  _hWnd = NULL;
}

HWND Window::Create(HINSTANCE hInst, int PosX, int PosY, int Width, int Height)
{
  WNDCLASSW WinClass;

  _hInst = hInst;
  _width = Width;
  _height = Height;

  WinClass.style          = 0;
  WinClass.lpfnWndProc    = windowProc;
  WinClass.cbClsExtra     = 0;
  WinClass.cbWndExtra     = 0;
  WinClass.hInstance      = _hInst;
  WinClass.hIcon          = LoadIcon(_hInst, MAKEINTRESOURCE(IDI_APPLICATION));
  WinClass.hCursor        = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
  WinClass.hbrBackground  = (HBRUSH)0;
  WinClass.lpszMenuName   = 0;
  WinClass.lpszClassName  = L"DWindow";

  if (!RegisterClassW(&WinClass))
  {
    Log::GetLog()->WriteToLog(L"Error registration window (DWindow.cpp)");
    return FALSE;
  }

  _hWnd = 
    CreateWindowExW( 
    WS_EX_CLIENTEDGE, L"DWindow", L"", 
    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
    PosX, PosY, Width, Height, 
    NULL, NULL, _hInst, (LPVOID)this
    );

  ShowWindow(_hWnd, SW_SHOW);

  return _hWnd;
}

LRESULT CALLBACK Window::windowProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
  switch(Message)
  {
  case WM_NCCREATE :
    {
      Window* MainWnd = (Window *)(((LPCREATESTRUCT)lParam)->lpCreateParams);
      MainWnd->_hWnd = hWnd;
      SetWindowLong(hWnd, GWLP_USERDATA, (LONG)MainWnd);
      return TRUE;
    }
  default:
    {
      return ((Window*)GetWindowLong(hWnd, GWLP_USERDATA))->msgProc(hWnd, Message, wParam, lParam);
    }
  }
}

LRESULT CALLBACK Window::msgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
  switch(Message)
  {
  case WM_PAINT:
    {
      _hdc = BeginPaint( hWnd, &_ps );
      EndPaint( hWnd, &_ps );
      break;
    }

  case WM_KEYDOWN:
    {
      if (wParam == VK_ESCAPE)
      {
        PostQuitMessage(0);
        break;
      }
      break;
    }
  case WM_DESTROY:
    {
      PostQuitMessage(0);
      break;
    }
  }

  return DefWindowProc(hWnd, Message, wParam, lParam);
}

