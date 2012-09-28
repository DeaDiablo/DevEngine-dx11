#ifndef DEV_RENDER_H
#define DEV_RENDER_H

#include <Core/DSystemLib.h>
#include <Element/DScene.h>
#include <GUI/DWindow.h>
#include <Input/DOutputClass.h>

namespace dev
{
  class Render
  {
  public:
    Render(int widthWnd, int heightWnd);
    Render(HWND hWnd);
    Render(HINSTANCE hInstance, int PosX, int PosY, int Width, int Height);
    virtual ~Render();

    bool InitRender(int width, int height, int RefreshHz, bool FullScreenMode);
    void Destroy();
    void Release();

    void SetScene(Scene* scene);
    inline Scene* GetScene() const
    {
      return _scene;
    }

    void Run();
    void Frame();
    void OutputUpdate();

    void RunThread();
    void StopThread();

    void RegisterOutputClass(OutputClass* outputClass);
    void UnregisterOutputClass(OutputClass* outputClass);

    void RegisterVS(VertexShader* vs);
    void RegisterPS(PixelShader* ps);

  protected:
    Window*   _wnd;
    HWND      _hWnd;

    Scene*    _scene;
    OutputClasses::Set _outputs;

    HANDLE    _renderThread;
    bool      _stopRender;

    Input              _input;
    CriticalSection    _cs;
    int                _frame;

    static void startRender(LPVOID param);
    void  runRender();
  };
}

#endif