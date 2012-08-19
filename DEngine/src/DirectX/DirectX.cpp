#include <DX/DirectX.h>

#include <Core/DLog.h>

using namespace dev;

DirectXClass::DirectXClass() :
  _dxDevice(NULL),
  _dxDeviceContext(NULL),
  _dxSwapChain(NULL),
  _dxRenderTargetView(NULL),
  _dxDepthStencilView(NULL),
  _dxDepthStencilState(NULL),
  _dxRasterState(NULL),
  _featureLevel(D3D_FEATURE_LEVEL_11_0),
  
  _initialized(FALSE),

  _width(0),
  _height(0),
  _msMode(0),
  _refreshHz(0),
  _fullScreenMode(FALSE)
{
}

DirectXClass::~DirectXClass()
{
  DestroyDirectX();
}

bool DirectXClass::InitDirectX(HWND hWnd, bool fullScreenMode, UINT msMode, UINT width, UINT height, UINT refreshHz)
{
  _hWnd = hWnd;
  _fullScreenMode = fullScreenMode;
  _msMode = msMode;
  _width = width;
  _height = height;
  _refreshHz = refreshHz;
  if (!CreateDeviceDX())
    return FALSE;

  if (!CreateRenderTargetDX())
    return FALSE;

  _dxDepthStencilView = CreateDepthStencilView();
  if (!_dxDepthStencilView)
    return FALSE;
  SetDepthStencilView(_dxDepthStencilView);
  
  _dxDepthStencilState = CreateDepthStencilState();
  if (!_dxDepthStencilState)
    return FALSE;
  SetDepthStencilState(_dxDepthStencilState);
  
  CreateViewPort((float)width, (float)height);

  //_dxDeviceContext->RSSetState(_dxRasterState);

  _dxDeviceContext->RSSetViewports(1, &_viewPort);
  _dxDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  _initialized = TRUE;

  return TRUE;
}

bool DirectXClass::CreateDeviceDX()
{
  DXGI_SWAP_CHAIN_DESC scd;

  ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

  scd.BufferCount = 1;
  scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  scd.BufferDesc.Width  = _width;
  scd.BufferDesc.Height = _height;
  scd.BufferDesc.RefreshRate.Numerator = _refreshHz;
  scd.BufferDesc.RefreshRate.Denominator = 1;

  scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  scd.OutputWindow = _hWnd;
  scd.SampleDesc.Count = _msMode;
  scd.SampleDesc.Quality = 0;
  scd.Windowed = !_fullScreenMode;

  UINT creationFlags = 0;
#ifdef _DEBUG
  creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_FEATURE_LEVEL featureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0
  };
  UINT numFeatureLevels = ARRAYSIZE( featureLevels );

  if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 
    creationFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, 
    &scd, &_dxSwapChain, &_dxDevice, &_featureLevel, &_dxDeviceContext)))
  {
    Log::GetLog()->WriteToLog(L"Failed device init DirectX");
    _dxSwapChain = NULL;
    _dxDevice = NULL;
    _dxDeviceContext = NULL;
    return FALSE;
  }

  Log::GetLog()->WriteToLog(L"Init device DirectX");
  return TRUE;
}

bool DirectXClass::CreateRenderTargetDX()
{
  ID3D11Texture2D* backBuffer = NULL;
  if(FAILED(_dxSwapChain->GetBuffer(0, __uuidof( ID3D11Texture2D ), (LPVOID*)&backBuffer)))
  {
    Log::GetLog()->WriteToLog(L"Failed back buffer init");
    return FALSE;
  }

  HRESULT hr = _dxDevice->CreateRenderTargetView(backBuffer, NULL, &_dxRenderTargetView);
  backBuffer->Release();
  if(FAILED(hr))
  {
    Log::GetLog()->WriteToLog(L"Failed init RenderTargetView");
    return FALSE;
  }
  
  Log::GetLog()->WriteToLog(L"Created RenderTargetView");
  
  return TRUE;
}

bool DirectXClass::CreateRasterizerState()
{
  D3D11_RASTERIZER_DESC rasterDesc;
  rasterDesc.AntialiasedLineEnable = FALSE;
  rasterDesc.CullMode = D3D11_CULL_NONE;
  rasterDesc.DepthBias = 0;
  rasterDesc.DepthBiasClamp = 0.0f;
  rasterDesc.DepthClipEnable = TRUE;
  rasterDesc.FillMode = D3D11_FILL_SOLID;
  rasterDesc.FrontCounterClockwise = FALSE;
  rasterDesc.MultisampleEnable = TRUE;
  rasterDesc.ScissorEnable = FALSE;
  rasterDesc.SlopeScaledDepthBias = 0.0f;

  // Create the rasterizer state from the description we just filled out.
  if (FAILED(_dxDevice->CreateRasterizerState(&rasterDesc, &_dxRasterState)))
  {
    Log::GetLog()->WriteToLog(L"RasterizerState not init");
    return FALSE;
  }

  return TRUE;
}

void DirectXClass::CreateViewPort(float width, float height)
{
  _viewPort.Width = width;
  _viewPort.Height = height;
  _viewPort.MinDepth = 0.0f;
  _viewPort.MaxDepth = 1.0f;
  _viewPort.TopLeftX = 0;
  _viewPort.TopLeftY = 0;
}

void DirectXClass::DestroyDirectX()
{
  _initialized = FALSE;

  DESTROY_DX(_dxSwapChain)
  ClearDSBuffer();
  
  if (_dxDeviceContext)
  {
    _dxDeviceContext->ClearState();
    _dxDeviceContext->Release();
    _dxDeviceContext = NULL;
  }

  DESTROY_DX(_dxDevice)

  destroyShaderManager();
  destroyTextureManager();

  Log::GetLog()->WriteToLog(L"Release DirectX");
}

//Depth and Stencil buffers
void DirectXClass::ClearDSBuffer()
{
  for(DSViewSet::iterator i = _dsViewVec.begin(); i != _dsViewVec.end(); ++i)
  {
    ID3D11DepthStencilView* element = (*i);
    DESTROY_DX(element);
  }

  for(DSTex2DSet::iterator i = _dsTex2DVec.begin(); i != _dsTex2DVec.end(); ++i)
  {
    ID3D11Texture2D* element = (*i);
    DESTROY_DX(element);
  }

  for(DSStateSet::iterator i = _dsStateVec.begin(); i != _dsStateVec.end(); ++i)
  {
    ID3D11DepthStencilState* element = (*i);
    DESTROY_DX(element);
  }
}

void DirectXClass::SetDepthStencilView(ID3D11DepthStencilView* dsView)
{
  _dxDepthStencilView = dsView;
  _dxDeviceContext->OMSetRenderTargets(1, &_dxRenderTargetView, _dxDepthStencilView);
}

void DirectXClass::SetDepthStencilState(ID3D11DepthStencilState* dsState)
{
  _dxDepthStencilState = dsState;
  _dxDeviceContext->OMSetDepthStencilState(_dxDepthStencilState, 1);
}

ID3D11DepthStencilView* DirectXClass::CreateDepthStencilView()
{
  ID3D11Texture2D* dxDepthStencil = NULL;
  // Create depth stencil texture
  D3D11_TEXTURE2D_DESC descDepth;
  ZeroMemory( &descDepth, sizeof(descDepth) );
  descDepth.Width = _width;
  descDepth.Height = _height;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  descDepth.SampleDesc.Count = _msMode;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D11_USAGE_DEFAULT;
  descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  descDepth.CPUAccessFlags = 0;
  descDepth.MiscFlags = 0;
  if (FAILED(_dxDevice->CreateTexture2D(&descDepth, NULL, &dxDepthStencil)))
  {
    Log::GetLog()->WriteToLog(L"DepthStencil not init");
    return FALSE;
  }

  _dsTex2DVec.insert(dxDepthStencil);

  ID3D11DepthStencilView* dxDepthStencilView = NULL;

  // Create the depth stencil view
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  ZeroMemory( &descDSV, sizeof(descDSV) );
  descDSV.Format = descDepth.Format;
  if( _msMode > 1 )
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
  else
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;
  if (FAILED(_dxDevice->CreateDepthStencilView(dxDepthStencil, &descDSV, &dxDepthStencilView)))
  {
    Log::GetLog()->WriteToLog(L"DepthStencilView not init");
    return NULL;
  }

  _dsViewVec.insert(dxDepthStencilView);

  return dxDepthStencilView;
}

ID3D11DepthStencilState* DirectXClass::CreateDepthStencilState(bool depthEnable, D3D11_DEPTH_WRITE_MASK depthMask, D3D11_COMPARISON_FUNC depthComparison)
{
  D3D11_DEPTH_STENCIL_DESC dsDesc;

  // Depth test parameters
  dsDesc.DepthEnable = depthEnable;
  dsDesc.DepthWriteMask = depthMask;
  dsDesc.DepthFunc = depthComparison;

  // Stencil test parameters
  dsDesc.StencilEnable = FALSE;
  dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
  dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

  // Stencil operations if pixel is front-facing
  dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  // Stencil operations if pixel is back-facing
  dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  ID3D11DepthStencilState* dxDepthStencilState = NULL;

  if (FAILED(_dxDevice->CreateDepthStencilState(&dsDesc, &dxDepthStencilState)))
  {
    Log::GetLog()->WriteToLog(L"DepthStencilState not created");
    return NULL;
  }

  _dsStateVec.insert(dxDepthStencilState);

  return dxDepthStencilState;
}

//DRAW
void DirectXClass::ClearFrame()
{
  _dxDeviceContext->ClearRenderTargetView(_dxRenderTargetView, clearColor);
  _dxDeviceContext->ClearDepthStencilView(_dxDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DirectXClass::Present(bool vSync)
{
  _dxSwapChain->Present(vSync, 0);
}

//SHADER MANAGER
void DirectXClass::destroyShaderManager()
{
  for(ShaderMap::iterator i = _vertexShaders.begin(); i != _vertexShaders.end(); ++i)
  {
    delete (*i).second->vertexShader;
    delete (*i).second;
  }

  for(ShaderMap::iterator i = _pixelShaders.begin(); i != _pixelShaders.end(); ++i)
  {
    delete (*i).second->pixelShader;
    delete (*i).second;
  }

  _vertexShaders.clear();
  _pixelShaders.clear();
}

VertexShader* DirectXClass::GetVertexShader(const wchar_t* path, VertexShader::TypeVertexShader type, const char* funcName)
{
  int hash = getShaderHash(path, type, funcName);
  ShaderMap::iterator i = _vertexShaders.find(hash);
  if (i == _vertexShaders.end())
  {
    _buffer = new ShaderStruct();
    _buffer->vertexShader = new VertexShader(path, type, funcName);
    if (!_buffer->vertexShader->CompileShader())
    {
      delete _buffer->vertexShader;
      delete _buffer;
      return NULL;
    }
    _buffer->count = 1;
    _vertexShaders[hash] = _buffer;
    return _buffer->vertexShader;
  }

  (*i).second->count++;
  return (*i).second->vertexShader;
}

PixelShader* DirectXClass::GetPixelShader(const wchar_t* path, PixelShader::TypePixelShader type, const char* funcName)
{
  int hash = getShaderHash(path, type, funcName);
  ShaderMap::iterator i = _pixelShaders.find(hash);
  if (i == _pixelShaders.end())
  {
    _buffer = new ShaderStruct();
    _buffer->pixelShader = new PixelShader(path, type, funcName);
    if (!_buffer->pixelShader->CompileShader())
    {
      delete _buffer->pixelShader;
      delete _buffer;
      return NULL;
    }
    _buffer->count = 1;
    _pixelShaders[hash] = _buffer;
    return _buffer->pixelShader;
  }

  (*i).second->count++;
  return (*i).second->pixelShader;
}

void DirectXClass::RemoveVertexShader(VertexShader* shader)
{
  if (!shader)
    return;

  for (ShaderMap::iterator i = _vertexShaders.begin(); i != _vertexShaders.end(); ++i)
  {
    if ((*i).second->vertexShader == shader)
    {
      (*i).second->count--;
      if ((*i).second->count < 1)
      {
        delete (*i).second->vertexShader;
        delete (*i).second;
        _vertexShaders.erase(i);
      }
      return;
    }
  }
}

void DirectXClass::RemovePixelShader(PixelShader* shader)
{
  if (!shader)
    return;

  for (ShaderMap::iterator i = _pixelShaders.begin(); i != _pixelShaders.end(); ++i)
  {
    if ((*i).second->pixelShader == shader)
    {
      (*i).second->count--;
      if ((*i).second->count < 1)
      {
        delete (*i).second->pixelShader;
        delete (*i).second;
        _pixelShaders.erase(i);
      }
      return;
    }
  }
}

UINT DirectXClass::getShaderHash(const wchar_t* path, DWORD type, const char* funcName)
{
  UINT hash = 0;
  size_t len = strlen(funcName);
  for(UINT i = 0; i < wcslen(path); i++)
    hash += (path[i] + i + type + (funcName[i % len])) << (i % 32);
  return hash;
}

void DirectXClass::destroyTextureManager()
{
  for(TextureMap::iterator i = _textures.begin(); i != _textures.end(); i++)
    delete (*i).second.texture;
  _textures.clear();

  for(SamplerSet::iterator i = _samplers.begin(); i != _samplers.end(); i++)
  {
    ID3D11SamplerState* sampler = (*i);
    DESTROY_DX(sampler);
  }
  _samplers.clear();
}

Texture* DirectXClass::GetTexture(const wchar_t* path)
{
  int hash = getTextureHash(path);

  TextureMap::iterator i = _textures.find(hash);
  if (i == _textures.end())
  {
    TextureStruct buffer;
    buffer.texture = new Texture(path);
    buffer.count = 1;
    _textures[hash] = buffer;
    return buffer.texture;
  }

  (*i).second.count++;
  return (*i).second.texture;
}

ID3D11SamplerState* DirectXClass::GetSamplerState()
{
  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory( &sampDesc, sizeof(sampDesc) );
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

  ID3D11SamplerState* state = NULL;
  if (FAILED(_dxDevice->CreateSamplerState(&sampDesc, &state)))
  {
    Log::GetLog()->WriteToLog(L"SamplerState not created");
    return NULL;
  }
  
  _samplers.insert(state);
  return state;
}

void DirectXClass::RemoveTexture(const wchar_t* path)
{
  int hash = getTextureHash(path);
  if (_textures[hash].texture)
  {
    _textures[hash].count--;
    if (_textures[hash].count < 1)
    {
      delete _textures[hash].texture;
      _textures.erase(hash);
    }      
    return;
  }
}

void DirectXClass::RemoveTexture(Texture* texture)
{
  for (TextureMap::iterator i = _textures.begin(); i != _textures.end(); ++i)
  {
    if ((*i).second.texture == texture)
    {
      (*i).second.count--;
      if ((*i).second.count < 1)
      {
        delete (*i).second.texture;
        _textures.erase(i);
      }
      return;
    }
  }
}

UINT DirectXClass::getTextureHash(const wchar_t* path)
{
  UINT hash = 0;
  for(UINT i = 0; i < wcslen(path); i++)
    hash += (path[i] + i) << (i % 32);
  return hash;
}
