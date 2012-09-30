#include <DX/DirectX.h>

#include <Core/DLog.h>

using namespace dev;

ID3D11Device*            DirectX::_dxDevice         = NULL;
ID3D11DeviceContext*     DirectX::_dxDeviceContext  = NULL;

DirectX::DirectX() :
  //setup DirectX
  _dxSwapChain(NULL),
  _initialized(FALSE),
  _width(0),
  _height(0),
  _msMode(0),
  _refreshHz(0),
  _fullScreenMode(FALSE),
  _featureLevel(D3D_FEATURE_LEVEL_11_0),

  //render targets
  _screenRenderTarget(NULL),
  _activeRenderTargetView(NULL),
  _numRTV(0),

  //depth and stencil targets
  _screenDepthStencilTarget(NULL),
  _activeDepthStencilView(NULL),
  _activeDepthStencilState(NULL),

  //raster states
  _activeRasterState(NULL)
{
}

DirectX::~DirectX()
{
  DestroyDirectX();
}

//********************************************************************************************************************//
//**************************************************SETUP DIRECTX*****************************************************//
//********************************************************************************************************************//
bool DirectX::InitDirectX(HWND hWnd, bool fullScreenMode, UINT msMode, UINT width, UINT height, UINT refreshHz)
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
  _dxDeviceContext->RSSetViewports(1, &_viewPort);
  _dxDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  //SetDepthStencilState(CreateDepthStencilState(TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS));
  //SetRasterizerState(CreateRasterizerState());

  _initialized = TRUE;

  return TRUE;
}

bool DirectX::CreateDeviceDX()
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

void DirectX::DestroyDirectX()
{
  _initialized = FALSE;

  DESTROY_DX(_dxSwapChain);
  DESTROY_DX(_dxDeviceContext);
  DESTROY_DX(_dxDevice);
  destroyRenderTargets();
  destroyDSBuffer();
  destroyShaderManager();
  destroyTextureManager();
  
  WRITE_LOG(L"Release DirectX");
}

//********************************************************************************************************************//
//************************************************VIEW PORTS**********************************************************//
//********************************************************************************************************************//
void DirectX::CreateViewPort(float width, float height)
{
  _viewPort.Width = width;
  _viewPort.Height = height;
  _viewPort.MinDepth = 0.0f;
  _viewPort.MaxDepth = 1.0f;
  _viewPort.TopLeftX = 0;
  _viewPort.TopLeftY = 0;
}

//********************************************************************************************************************//
//**********************************************RENDER TARGETS********************************************************//
//********************************************************************************************************************//
ID3D11RenderTargetView* DirectX::CreateScreenRenderTarget()
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

ID3D11RenderTargetView* DirectX::CreateTextureRenderTarget(int targetNum, DXGI_FORMAT format, bool recreate)
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

void DirectX::SetRenderTargets(int num, ID3D11RenderTargetView* const* rtv, ID3D11DepthStencilView* dsv)
{
  if (num >= D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
    return;

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

void DirectX::destroyRenderTargets()
{
  for(RenderTargetMap::iterator i = _renderTargets.begin(); i != _renderTargets.end(); ++i)
  {
    DESTROY_DX((*i).second->renderTexture);
    DESTROY_DX((*i).second->renderTargetView);
    DESTROY_DX((*i).second->shaderResource);
    delete (*i).second;
  }
  _renderTargets.clear();

  if (_screenRenderTarget)
  {
    DESTROY_DX(_screenRenderTarget->renderTexture);
    DESTROY_DX(_screenRenderTarget->renderTargetView);
    delete _screenRenderTarget;
    _screenRenderTarget = NULL;
  }
}

//********************************************************************************************************************//
//*****************************************DEPTH AND STENCIL TARGETS**************************************************//
//********************************************************************************************************************//
void DirectX::SetDepthStencilState(ID3D11DepthStencilState* dsState)
{
  if (_activeDepthStencilState == dsState)
    return;

  _activeDepthStencilState = dsState;
  _dxDeviceContext->OMSetDepthStencilState(_activeDepthStencilState, 1);
}

ID3D11DepthStencilView* DirectX::CreateScreenDepthStencilView()
{
  if (!_screenDepthStencilTarget)
    _screenDepthStencilTarget = newDepthStencilTarget();

  return _screenDepthStencilTarget->depthStencilView;
}

ID3D11DepthStencilView* DirectX::CreateDepthStencilView(int targetNum)
{
  if (_depthStencilTargets.find(targetNum) != _depthStencilTargets.end())
    return _depthStencilTargets[targetNum]->depthStencilView;

  _depthStencilTargets[targetNum] = newDepthStencilTarget();
  return _depthStencilTargets[targetNum]->depthStencilView;
}

ID3D11DepthStencilState* DirectX::CreateDepthStencilState(bool depthEnable, D3D11_DEPTH_WRITE_MASK depthMask, D3D11_COMPARISON_FUNC depthComparison)
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

DirectX::DepthStencilTarget* DirectX::newDepthStencilTarget()
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
  descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
  descDepth.SampleDesc.Count = _msMode;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D11_USAGE_DEFAULT;
  descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
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
  descDSV.Format = DXGI_FORMAT_D32_FLOAT;
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

  // create shader resource view
  D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
  ZeroMemory(&resourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
  resourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
  if (_msMode > 1)
    resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
  else
    resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  resourceViewDesc.Texture2D.MipLevels = 1;

  if(FAILED(_dxDevice->CreateShaderResourceView(dst->depthStencilTexture, &resourceViewDesc, &dst->shaderResource)))
  {
    WRITE_LOG(L"Failed init ShaderResource for DepthStencilView");
    return NULL;
  }

  return dst;
}

void DirectX::destroyDSBuffer()
{
  for(DepthStencilTargetMap::iterator i = _depthStencilTargets.begin(); i != _depthStencilTargets.end(); ++i)
  {
    DESTROY_DX((*i).second->depthStencilTexture);
    DESTROY_DX((*i).second->depthStencilView);
    delete (*i).second;
  }

  if (_screenDepthStencilTarget)
  {
    DESTROY_DX(_screenDepthStencilTarget->depthStencilTexture);
    DESTROY_DX(_screenDepthStencilTarget->depthStencilView);
    delete _screenDepthStencilTarget;
    _screenDepthStencilTarget = NULL;
  }

  for(DSStateSet::iterator i = _dsStateVec.begin(); i != _dsStateVec.end(); ++i)
    DESTROY_DX((*i));
}

//********************************************************************************************************************//
//***********************************************RASTER STATES********************************************************//
//********************************************************************************************************************//
ID3D11RasterizerState* DirectX::CreateRasterizerState()
{
  ID3D11RasterizerState* rsState = NULL;
  
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
  if (FAILED(_dxDevice->CreateRasterizerState(&rasterDesc, &rsState)))
  {
    WRITE_LOG(L"RasterizerState not init");
    return NULL;
  }

  _rsStateVec.insert(rsState);

  return rsState;
}

void DirectX::SetRasterizerState(ID3D11RasterizerState* rsState)
{
  if (_activeRasterState == rsState)
    return;

  _activeRasterState = rsState;
  _dxDeviceContext->RSSetState(_activeRasterState);
}

void DirectX::destroyRSStates()
{
  for(RSStateSet::iterator i = _rsStateVec.begin(); i != _rsStateVec.end(); ++i)
    DESTROY_DX((*i));
}

//********************************************************************************************************************//
//***********************************************DRAW DIRECTX*********************************************************//
//********************************************************************************************************************//
void DirectX::ClearAllRenderTargets()
{
  for(RenderTargetMap::iterator i = _renderTargets.begin(); i != _renderTargets.end(); ++i)
    _dxDeviceContext->ClearRenderTargetView((*i).second->renderTargetView, clearColor);
  if (_screenRenderTarget->renderTargetView)
    _dxDeviceContext->ClearRenderTargetView(_screenRenderTarget->renderTargetView, clearColor);
}

void DirectX::ClearAllDepthStencil()
{
  for(DepthStencilTargetMap::iterator i = _depthStencilTargets.begin(); i != _depthStencilTargets.end(); ++i)
    _dxDeviceContext->ClearDepthStencilView((*i).second->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
  if (_screenDepthStencilTarget->depthStencilView)
    _dxDeviceContext->ClearDepthStencilView(_screenDepthStencilTarget->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DirectX::ClearActiveRenderTargets()
{
  if (_activeRenderTargetView)
  {
    for (int i = 0; i < _numRTV; i++)
      _dxDeviceContext->ClearRenderTargetView(_activeRenderTargetView[i], clearColor);
  }
}

void DirectX::ClearActiveDepthStencil()
{
  if (_activeDepthStencilView)
    _dxDeviceContext->ClearDepthStencilView(_activeDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DirectX::Present(bool vSync)
{
  _dxSwapChain->Present(vSync, 0);
}

//********************************************************************************************************************//
//**************************************************SHADERS***********************************************************//
//********************************************************************************************************************//
void DirectX::VSSetConstantBuffers(UINT startSlot, UINT num, ID3D11Buffer* const* buffer)
{
  _dxDeviceContext->VSSetConstantBuffers(startSlot, num, buffer);
}

void DirectX::PSSetConstantBuffers(UINT startSlot, UINT num, ID3D11Buffer* const* buffer)
{
  _dxDeviceContext->VSSetConstantBuffers(startSlot, num, buffer);
}

void DirectX::destroyShaderManager()
{
  for(ShaderMap::iterator i = _shaders.begin(); i != _shaders.end(); ++i)
    delete (*i).second;

  _shaders.clear();
}

UINT DirectX::getShaderHash(const wchar_t* path, DWORD type, const char* funcName)
{
  UINT hash = 0;
  size_t len = strlen(funcName);
  for(UINT i = 0; i < wcslen(path); i++)
    hash += (path[i] + i + type + (funcName[i % len])) << (i % 32);
  return hash;
}

//********************************************************************************************************************//
//**********************************************TEXTURE MANAGER*******************************************************//
//********************************************************************************************************************//
Texture* DirectX::GetTexture(const wchar_t* path)
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

ID3D11SamplerState* DirectX::GetSamplerState()
{
  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory( &sampDesc, sizeof(sampDesc) );
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
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

void DirectX::RemoveTexture(const wchar_t* path)
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

void DirectX::RemoveTexture(Texture* texture)
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

void DirectX::destroyTextureManager()
{
  for(TextureMap::iterator i = _textures.begin(); i != _textures.end(); ++i)
    delete (*i).second.texture;
  _textures.clear();

  for(SamplerSet::iterator i = _samplers.begin(); i != _samplers.end(); ++i)
    DESTROY_DX((*i));

  _samplers.clear();
}

UINT DirectX::getTextureHash(const wchar_t* path)
{
  UINT hash = 0;
  for(UINT i = 0; i < wcslen(path); i++)
    hash += (path[i] + i) << (i % 32);
  return hash;
}
