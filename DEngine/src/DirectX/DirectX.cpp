#include <DX/DirectX.h>

#include <Core/DLog.h>

using namespace dev;

DirectXClass::DirectXClass() :
  _dxDevice(NULL),
  _dxDeviceContext(NULL),
  _dxSwapChain(NULL),
  _activeRenderTargetView(NULL),
  _activeDepthStencilView(NULL),
  _activeDepthStencilState(NULL),
  _dxRasterState(NULL),
  _featureLevel(D3D_FEATURE_LEVEL_11_0),
  
  _initialized(FALSE),
  _screenRenderTarget(NULL),
  _screenDepthStencilTarget(NULL),

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

  if (!CreateScreenRenderTarget() || !CreateScreenDepthStencilView())
    return FALSE;
  
  CreateViewPort((float)width, (float)height);

  _dxDeviceContext->OMSetRenderTargets(1, &_screenRenderTarget->renderTargetView, _screenDepthStencilTarget->depthStencilView);
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
    WRITE_LOG(L"Failed device init DirectX");
    _dxSwapChain = NULL;
    _dxDevice = NULL;
    _dxDeviceContext = NULL;
    return FALSE;
  }

  WRITE_LOG(L"Init device DirectX");
  return TRUE;
}

ID3D11RenderTargetView* DirectXClass::CreateScreenRenderTarget()
{
  if (_screenRenderTarget)
    return _screenRenderTarget->renderTargetView;

  _screenRenderTarget = new RenderTarget();
  _screenRenderTarget->renderTexture = NULL;
  if(FAILED(_dxSwapChain->GetBuffer(0, __uuidof( ID3D11Texture2D ), (LPVOID*)&_screenRenderTarget->renderTexture)))
  {
    Log::GetLog()->WriteToLog(L"Failed back buffer init");
    return NULL;
  }

  HRESULT hr = _dxDevice->CreateRenderTargetView(_screenRenderTarget->renderTexture, NULL, &_screenRenderTarget->renderTargetView);

  if(FAILED(hr))
  {
    Log::GetLog()->WriteToLog(L"Failed init RenderTargetView");
    return NULL;
  }

  Log::GetLog()->WriteToLog(L"Created screen RenderTargetView");

  return _screenRenderTarget->renderTargetView;
}

ID3D11RenderTargetView* DirectXClass::CreateTextureRenderTarget(int targetNum, DXGI_FORMAT format, bool recreate)
{
  if (!recreate && _renderTargets.find(targetNum) != _renderTargets.end())
    return _renderTargets[targetNum]->renderTargetView;

  RenderTarget* rt = new RenderTarget();
  rt->renderTexture = NULL;
  D3D11_TEXTURE2D_DESC textureDesc;
  // Initialize the render target texture description.
  ZeroMemory(&textureDesc, sizeof(textureDesc));

  // Setup the render target texture description.
  textureDesc.Width               = _width;
  textureDesc.Height              = _height;
  textureDesc.MipLevels           = 1;
  textureDesc.ArraySize           = 1;
  textureDesc.Format              = format;//DXGI_FORMAT_R32G32B32A32_FLOAT;
  textureDesc.SampleDesc.Count    = _msMode;
  textureDesc.SampleDesc.Quality  = 0;
  textureDesc.Usage               = D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags           = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  textureDesc.CPUAccessFlags      = 0;
  textureDesc.MiscFlags           = 0;

  if(FAILED(_dxDevice->CreateTexture2D(&textureDesc, NULL, &rt->renderTexture)))
  {
    WRITE_LOG(L"Failed render texture init");
    return NULL;
  }

  D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

  renderTargetViewDesc.Format = format;
  if (_msMode > 1)
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
  else
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  renderTargetViewDesc.Texture2D.MipSlice = 0;

  if(FAILED(_dxDevice->CreateRenderTargetView(rt->renderTexture, NULL, &rt->renderTargetView)))
  {
    WRITE_LOG(L"Failed init RenderTargetView");
    return NULL;
  }

  // create shader resource view
  D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
  ZeroMemory(&resourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
  resourceViewDesc.Format = format;
  if (_msMode > 1)
    resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
  else
    resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  resourceViewDesc.Texture2D.MipLevels = 1;

  if(FAILED(_dxDevice->CreateShaderResourceView(rt->renderTexture, &resourceViewDesc, &rt->shaderResource)))
  {
    WRITE_LOG(L"Failed init ShaderResource for RenderTargetView");
    return NULL;
  }

  WRITE_LOG(L"Created texture RenderTargetView");

  _renderTargets[targetNum] = rt;

  return rt->renderTargetView;
}

void DirectXClass::SetRenderTargets(int num, ID3D11RenderTargetView** rtv, ID3D11DepthStencilView* dsv)
{
  bool active = TRUE;

  if (_numRTV == num && _activeDepthStencilView == dsv)
  {
    for (int i = 0; i < num; i++)
    {
      if (_activeRenderTargetView[i] != rtv[i])
      {
        active = FALSE;
        break;
      }
    }
  }
  else
    active = FALSE;

  if (active)
    return;

  _numRTV = num;
  _activeRenderTargetView = rtv;
  _activeDepthStencilView = dsv;
  _dxDeviceContext->OMSetRenderTargets(_numRTV, _activeRenderTargetView, _activeDepthStencilView);
}

void DirectXClass::destroyRenderTargets()
{
  for(RenderTargetMap::iterator i = _renderTargets.begin(); i != _renderTargets.end(); ++i)
  {
    DESTROY_DX((*i).second->renderTexture)
    DESTROY_DX((*i).second->renderTargetView)
    DESTROY_DX((*i).second->shaderResource)
    delete (*i).second;
  }
  _renderTargets.clear();

  if (_screenRenderTarget)
  {
    DESTROY_DX(_screenRenderTarget->renderTexture)
    DESTROY_DX(_screenRenderTarget->renderTargetView)
    delete _screenRenderTarget;
    _screenRenderTarget = NULL;
  }
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
    WRITE_LOG(L"RasterizerState not init");
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
  destroyRenderTargets();
  destroyDSBuffer();
  
  if (_dxDeviceContext)
  {
    _dxDeviceContext->ClearState();
    _dxDeviceContext->Release();
    _dxDeviceContext = NULL;
  }

  DESTROY_DX(_dxDevice)

  destroyShaderManager();
  destroyTextureManager();

  WRITE_LOG(L"Release DirectX");
}

DirectXClass::DepthStencilTarget* DirectXClass::newDepthStencilTarget()
{
  DepthStencilTarget* dst = new DepthStencilTarget();
  dst->depthStencilTexture = NULL;
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
  if (FAILED(_dxDevice->CreateTexture2D(&descDepth, NULL, &dst->depthStencilTexture)))
  {
    WRITE_LOG(L"DepthStencil not init");
    return FALSE;
  }

  ID3D11DepthStencilView* dxDepthStencilView = NULL;

  // Create the depth stencil view
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  ZeroMemory( &descDSV, sizeof(descDSV) );
  descDSV.Format = descDepth.Format;
  if(_msMode > 1)
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
  else
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;
  if (FAILED(_dxDevice->CreateDepthStencilView(dst->depthStencilTexture, &descDSV, &dst->depthStencilView)))
  {
    WRITE_LOG(L"DepthStencilView not init");
    return NULL;
  }
  return dst;
}

//Depth and Stencil buffers
void DirectXClass::destroyDSBuffer()
{
  for(DepthStencilTargetMap::iterator i = _depthStencilTargets.begin(); i != _depthStencilTargets.end(); ++i)
  {
    DESTROY_DX((*i).second->depthStencilTexture)
    DESTROY_DX((*i).second->depthStencilView)
    delete (*i).second;
  }

  if (_screenDepthStencilTarget)
  {
    DESTROY_DX(_screenDepthStencilTarget->depthStencilTexture)
    DESTROY_DX(_screenDepthStencilTarget->depthStencilView)
    delete _screenDepthStencilTarget;
    _screenDepthStencilTarget = NULL;
  }

  for(DSStateSet::iterator i = _dsStateVec.begin(); i != _dsStateVec.end(); ++i)
  {
    ID3D11DepthStencilState* element = (*i);
    DESTROY_DX(element)
  }
}

void DirectXClass::SetDepthStencilState(ID3D11DepthStencilState* dss)
{
  if (_activeDepthStencilState == dss)
    return;

  _activeDepthStencilState = dss;
  _dxDeviceContext->OMSetDepthStencilState(_activeDepthStencilState, 1);
}

ID3D11DepthStencilView* DirectXClass::CreateScreenDepthStencilView()
{
  if (!_screenDepthStencilTarget)
    _screenDepthStencilTarget = newDepthStencilTarget();
    
  return _screenDepthStencilTarget->depthStencilView;
}

ID3D11DepthStencilView* DirectXClass::CreateDepthStencilView(int targetNum)
{
  if (_depthStencilTargets.find(targetNum) != _depthStencilTargets.end())
    return _depthStencilTargets[targetNum]->depthStencilView;

  _depthStencilTargets[targetNum] = newDepthStencilTarget();
  return _depthStencilTargets[targetNum]->depthStencilView;
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
    WRITE_LOG(L"DepthStencilState not created");
    return NULL;
  }

  _dsStateVec.insert(dxDepthStencilState);

  return dxDepthStencilState;
}

//DRAW
void DirectXClass::ClearAllRenderTargets()
{
  for(RenderTargetMap::iterator i = _renderTargets.begin(); i != _renderTargets.end(); ++i)
    _dxDeviceContext->ClearRenderTargetView((*i).second->renderTargetView, clearColor);
  if (_screenRenderTarget->renderTargetView)
    _dxDeviceContext->ClearRenderTargetView(_screenRenderTarget->renderTargetView, clearColor);
}

void DirectXClass::ClearAllDepthStencil()
{
  for(DepthStencilTargetMap::iterator i = _depthStencilTargets.begin(); i != _depthStencilTargets.end(); ++i)
    _dxDeviceContext->ClearDepthStencilView((*i).second->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
  if (_screenDepthStencilTarget->depthStencilView)
    _dxDeviceContext->ClearDepthStencilView(_screenDepthStencilTarget->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DirectXClass::ClearActiveRenderTargets()
{
  if (_activeRenderTargetView)
  {
    for (int i = 0; i < _numRTV; i++)
      _dxDeviceContext->ClearRenderTargetView(_activeRenderTargetView[i], clearColor);
  }
}

void DirectXClass::ClearActiveDepthStencil()
{
  if (_activeDepthStencilView)
    _dxDeviceContext->ClearDepthStencilView(_activeDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DirectXClass::Present(bool vSync)
{
  _dxSwapChain->Present(vSync, 0);
}

//SHADER MANAGER
void DirectXClass::destroyShaderManager()
{
  for(ShaderMap::iterator i = _vertexShaders.begin(); i != _vertexShaders.end(); ++i)
    delete (*i).second;

  for(ShaderMap::iterator i = _pixelShaders.begin(); i != _pixelShaders.end(); ++i)
    delete (*i).second;

  _vertexShaders.clear();
  _pixelShaders.clear();
}

VertexShader* DirectXClass::GetVertexShader(const wchar_t* path, VertexShader::TypeVertexShader type, const char* funcName)
{
  int hash = getShaderHash(path, type, funcName);
  ShaderMap::iterator i = _vertexShaders.find(hash);
  if (i != _vertexShaders.end())
    return (*i).second->AsVertexShader();

  VertexShader* shader = new VertexShader(path, type, funcName);
  if (!shader->CompileShader())
  {
    delete shader;
    return NULL;
  }
  _vertexShaders[hash] = shader;
  return shader;
}

void DirectXClass::RegistrationVertexShader(VertexShader* vs)
{
  int hash = getShaderHash(vs->GetPath(), vs->GetType(), vs->GetFuncName());
  ShaderMap::iterator i = _vertexShaders.find(hash);
  if (i != _vertexShaders.end())
    return;

  if (!vs->IsCompiled())
    vs->CompileShader();
  _vertexShaders[hash] = vs;
  return;
}

PixelShader* DirectXClass::GetPixelShader(const wchar_t* path, PixelShader::TypePixelShader type, const char* funcName)
{
  int hash = getShaderHash(path, type, funcName);
  ShaderMap::iterator i = _pixelShaders.find(hash);
  if (i != _pixelShaders.end())
    return (*i).second->AsPixelShader();

  PixelShader* shader = new PixelShader(path, type, funcName);
  if (!shader->CompileShader())
  {
    delete shader;
    return NULL;
  }
  _pixelShaders[hash] = shader;
  return shader;
}

void DirectXClass::RegistrationPixelShader(PixelShader* ps)
{
  int hash = getShaderHash(ps->GetPath(), ps->GetType(), ps->GetFuncName());
  ShaderMap::iterator i = _pixelShaders.find(hash);
  if (i != _pixelShaders.end())
    return;

  if (!ps->IsCompiled())
    ps->CompileShader();
  _pixelShaders[hash] = ps;
  return;
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
  for(TextureMap::iterator i = _textures.begin(); i != _textures.end(); ++i)
    delete (*i).second.texture;
  _textures.clear();

  for(SamplerSet::iterator i = _samplers.begin(); i != _samplers.end(); ++i)
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
    WRITE_LOG(L"SamplerState not created");
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
