#include <Core/DRender.h>

#include <DX/DirectX.h>
#include <Core/DLog.h>
#include <process.h>

using namespace dev;

Render::Render(int width, int height) :
  _renderThread(INVALID_HANDLE_VALUE),
  _stopRender(FALSE),
  _scene(NULL),
  _input(NULL),
  _cs(NULL),
  _frame(0)
{
  _wnd = new Window();
  _hWnd = _wnd->Create(GetModuleHandle(0), CW_USEDEFAULT, CW_USEDEFAULT, width, height);

  if (!_hWnd)
  {
    Log::GetLog()->WriteToLog(L"Not _hWnd");
  }
}

Render::Render(HWND hWnd) :
  _hWnd(hWnd),
  _wnd(NULL),
  _renderThread(INVALID_HANDLE_VALUE),
  _stopRender(FALSE),
  _scene(NULL),
  _input(NULL),
  _cs(NULL),
  _frame(0)
{
}

Render::Render(HINSTANCE hInstance, int PosX, int PosY, int Width, int Height) :
  _renderThread(INVALID_HANDLE_VALUE),
  _stopRender(FALSE),
  _scene(NULL),
  _input(NULL),
  _cs(NULL),
  _frame(0)
{
  _wnd = new Window();
  _hWnd = _wnd->Create(hInstance, PosX, PosY, Width, Height);

  if (!_hWnd)
  {
    Log::GetLog()->WriteToLog(L"Not _hWnd");
  }
}

Render::~Render()
{
  Destroy();
}

void Render::Destroy()
{
  StopThread();
  
  if (_scene)
  {
    delete _scene;
    _scene = NULL;
  }

  if (_wnd)
  {
    SendMessage(_hWnd, WM_CLOSE, 0, 0);

    delete _wnd;
    _wnd = NULL;
  }
  _hWnd = NULL;

  DirectX::Release();
  SystemCS::Release();
  SystemInput::Release();
  SystemTimer::Release();
}

void Render::Release()
{
  delete this;
}

bool Render::InitRender(int width, int height, int RefreshHz, bool FullScreenMode)
{
  if (width == 0)
  {
    if (!FullScreenMode)
    {
      RECT rc;
      GetClientRect(_hWnd, &rc);
      width = rc.right - rc.left;
    }
    else
    {
      width = GetSystemMetrics(SM_CXSCREEN);
    }
  }
  
  if (height == 0)
  {
    if (!FullScreenMode)
    {
      RECT rc;
      GetClientRect(_hWnd, &rc);
      height = rc.bottom - rc.top;
    }
    else
    {
      height = GetSystemMetrics(SM_CYSCREEN);
    }
  }
  
  if (DX->InitDirectX(_hWnd, FullScreenMode, 1, width, height, RefreshHz))
  {
    _input  = SystemInput::Get();
    _cs     = SystemCS::Get();
    SystemTimer::Get()->Reset(1);
    SystemTimer::Get()->Reset(2);
    return TRUE;
  }

  return FALSE;
}

void Render::SetScene(Scene* scene)
{
  _scene = scene;
}

void Render::Run()
{
  if (!_hWnd || !DX->IsInitialized())
    return;

  MSG msg = {0};
  while( WM_QUIT != msg.message )
  {
    if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
      Frame();
  }
}

void Render::Frame()
{
  OutputUpdate();
  
  if (_scene)
  {
    _scene->Update();
    _scene->Draw(FALSE);
  }
   
  //show fps
  _frame++;
  if (SystemTimer::Get()->GetEventTimeS(2, 1.0))
  {
    wchar_t label[255];
    swprintf_s(label, 255, L"FPS: %d", _frame);
    SetWindowTextW(_hWnd, label);
    _frame = 0;
  }
  //
}

void Render::OutputUpdate()
{
  InputStruct is = _input->GetInputStruct();
  _cs->Lock();
  for(OCSet::iterator it = _outputs.begin(); it != _outputs.end(); ++it)
  {
    OutputClass* outputClass = (*it);
    if (outputClass->GetActive())
      outputClass->Handle(is, SystemTimer::Get()->GetDeltaTimeMS(1));
  }
  _cs->UnLock();
}

void Render::RunThread()
{
  if (DX->IsInitialized())
    _renderThread = (HANDLE)_beginthread(startRender, 0, (LPVOID)this);
}

void Render::StopThread()
{
  _stopRender = TRUE;
  if (_renderThread != INVALID_HANDLE_VALUE)
  {
    WaitForSingleObject(_renderThread, 10000);
    TerminateThread(_renderThread, 0);
    _renderThread = INVALID_HANDLE_VALUE;
    _stopRender = FALSE;
  }
}
void Render::startRender(LPVOID param)	  	
{
  Render* _render = static_cast<Render*>(param); 	
  _render->runRender();
}

void Render::runRender()
{
  while(!_stopRender)
  {
    Frame();
  }
  _endthread();
}

void Render::RegisterOutputClass(OutputClass* outputClass)
{
  _cs->Lock();
  _outputs.insert(outputClass);
  _cs->UnLock();
}

void Render::UnregisterOutputClass(OutputClass* outputClass)
{
  _cs->Lock();
  _outputs.erase(outputClass);
  _cs->UnLock();
}
