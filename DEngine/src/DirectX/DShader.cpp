#include <DX/DShader.h>

#include <Core/DLog.h>
#include <DX/DirectX.h>

using namespace dev;

Shader::Shader(const wchar_t* path, DWORD type, const char* functionName) :
  _blob(NULL),
  _path(path),
  _type(type),
  _function(functionName),
  _isCompiled(FALSE)
{
}

Shader::~Shader()
{
  releaseBlob();
}

bool Shader::compileShaderFromFile(const wchar_t* fileName, const char* functionName, const char* shaderModel)
{
  if (!supportTypeShader())
  {
    WRITE_LOG(L"Your videoadapter not supported this shader model");
    return FALSE;
  }

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
  #ifdef _DEBUG
    dwShaderFlags |= D3DCOMPILE_DEBUG;
  #endif

  ID3DBlob* pErrorBlob;
  if( FAILED(D3DX11CompileFromFileW(fileName, NULL, NULL, functionName, shaderModel, 
    dwShaderFlags, 0, NULL, &_blob, &pErrorBlob, NULL )) )
  {
    std::wstring buffer = L"Shader not compiled: ";
    buffer += fileName;
    WRITE_LOG(buffer.c_str());
    if(pErrorBlob != NULL)
    {
      char* error = (char*)pErrorBlob->GetBufferPointer();
      UINT strLen = (UINT)strlen(error);
      wchar_t* wError = new wchar_t[strLen];
      MultiByteToWideChar(CP_ACP, 0, error, strLen, wError, strLen);
      WRITE_LOG(wError);
    }
    else
      WRITE_LOG(L"Unknown error");
    if(pErrorBlob) 
      pErrorBlob->Release();
    return FALSE;
  }
  
  if(pErrorBlob) 
    pErrorBlob->Release();

  return TRUE;
}

//vertex shader
VertexShader::VertexShader(const wchar_t* path, TypeVertexShader type, const char* nameFunction) :
  Shader(path, (DWORD)type, nameFunction),
  _shader(NULL),
  _layout(NULL),
  _bufferType(Buffer::BT_NONE)
{
}

VertexShader::~VertexShader()
{
  _bufferType = Buffer::BT_NONE;

  if (_shader)
  {
    _shader->Release();
    _shader = NULL;
  }
  
  if (_layout)
  {
    _layout->Release();
    _layout = NULL;
  }
}

const char* VertexShader::getType()
{
  switch(_type)
  {
  case VS_1_1:
    return "vs_1_1";
  case VS_2_0:
    return "vs_2_0";
  case VS_2_A:
    return "vs_2_a";
  case VS_2_SW:
    return "vs_2_sw";
  case VS_3_0:
    return "vs_3_0";
  case VS_3_SW:
    return "vs_3_sw";
  case VS_4_0:
    return "vs_4_0";
  case VS_4_1:
    return "vs_4_1";
  case VS_5_0:
    return "vs_5_0";
  }
  return NULL;
}

bool VertexShader::supportTypeShader()
{
  switch(_type)
  {
  case VS_1_1:
  case VS_2_0:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_9_1;
  case VS_2_A:
  case VS_2_SW:
  case VS_3_0:
  case VS_3_SW:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_9_3;
  case VS_4_0:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_0;
  case VS_4_1:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_1;
  case VS_5_0:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0;
  }
  return false;
}

bool VertexShader::SetShader()
{
  if (!_shader || !_layout)
    return FALSE;
  
  DX_CONTEXT->VSSetShader(_shader, NULL, 0);
  DX_CONTEXT->IASetInputLayout(_layout);
  return TRUE;
}

bool VertexShader::CompileShader()
{
  if (!compileShaderFromFile(_path.c_str(), _function.c_str(), getType()))
    return FALSE;

  HRESULT hr = DX_DEVICE->CreateVertexShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), NULL, &_shader);

  if (FAILED(hr))
  {
    releaseBlob();
    WRITE_LOG(L"Vertex shader not created.");
    return FALSE;
  }

  _isCompiled = TRUE;
  return TRUE;
}

bool VertexShader::CreateLaout(Buffer::BufferType BT_Type)
{
  if (!_blob)
    return FALSE;
  
  _bufferType = BT_Type;

  HRESULT hr = DX_DEVICE->CreateInputLayout(Buffer::Declaration::GetLayout(_bufferType), Buffer::Declaration::GetSizeLayout(_bufferType), _blob->GetBufferPointer(), _blob->GetBufferSize(), &_layout);

  if (FAILED(hr))
  {
    WRITE_LOG(L"Layout not created.");
    return FALSE;
  }

  return TRUE;
}


//pixel shader
PixelShader::PixelShader(const wchar_t* path, TypePixelShader type, const char* nameFunction) :
  Shader(path, (DWORD)type, nameFunction),
  _shader(NULL)
{
  ClearAllRenderTargets();
  ClearResourcesRenderTarget();
  SetScreenDepthStencil();
  SetScreenRenderTarget(0);
}

PixelShader::~PixelShader()
{
  if (_shader)
  {
    _shader->Release();
    _shader = NULL;
  }
}

const char* PixelShader::getType()
{
  switch(_type)
  {
  case PS_2_0:
    return "ps_2_0";
  case PS_2_A:
    return "ps_2_a";
  case PS_2_B:
    return "ps_2_b";
  case PS_2_SW:
    return "ps_2_sw";
  case PS_3_0:
    return "ps_3_0";
  case PS_3_SW:
    return "ps_3_sw";
  case PS_4_0:
    return "ps_4_0";
  case PS_4_1:
    return "ps_4_1";
  case PS_5_0:
    return "ps_5_0";
  }
  return NULL;
}

bool PixelShader::supportTypeShader()
{
  switch(_type)
  {
  case PS_2_0:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_9_1;
  case PS_2_A:
  case PS_2_B:
  case PS_2_SW:
  case PS_3_0:
  case PS_3_SW:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_9_3;
  case PS_4_0:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_0;
  case PS_4_1:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_1;
  case PS_5_0:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0;
  }
  return false;
}

bool PixelShader::SetShader()
{
  if (!_shader)
    return FALSE;

  if (_numRTSlot > 0)
    DX.SetRenderTargets(_numRTSlot, _renderTargets, _depthStencilView);
  if (_numSRSlot > 0)
    DX_CONTEXT->PSSetShaderResources(_startSRSlot, _numSRSlot, _shaderResources);
  DX_CONTEXT->PSSetShader(_shader, NULL, 0);

  return TRUE;
}

bool PixelShader::CompileShader()
{
  if (!compileShaderFromFile(_path.c_str(), _function.c_str(), getType()))
    return FALSE;

  HRESULT hr = DX_DEVICE->CreatePixelShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), NULL, &_shader);
  
  releaseBlob();
  
  if (FAILED(hr))
  {
    WRITE_LOG(L"Pixel shader not created.");
    return FALSE;
  }

  _isCompiled = TRUE;
  return TRUE;
}

void PixelShader::SetScreenRenderTarget(int targetSlot)
{
  if (targetSlot < 0 || targetSlot >= MAX_RENDER_TARGETS)
    return;

  _renderTargets[targetSlot] = DX.CreateScreenRenderTarget();
  updateNumRenderTargets();
}

void PixelShader::SetResourceRenderTarget(int targetSlot, int numTargetInDX, DXGI_FORMAT format)
{
  if (targetSlot < 0 || targetSlot >= MAX_RENDER_TARGETS)
    return;

  _renderTargets[targetSlot] = DX.CreateTextureRenderTarget(numTargetInDX, format);
  updateNumRenderTargets();
}

void PixelShader::ClearRenderTarget(int targetSlot)
{
  if (targetSlot < 0 || targetSlot >= MAX_RENDER_TARGETS)
    return;

  if (!_renderTargets[targetSlot])
    return;

  _renderTargets[targetSlot] = NULL;
  updateNumRenderTargets();
}

void PixelShader::ClearAllRenderTargets()
{
  for (int i = 0; i < MAX_RENDER_TARGETS; i++)
    _renderTargets[i] = NULL;
  _numRTSlot = 0;
}

void PixelShader::SetScreenDepthStencil()
{
  _depthStencilView = DX.CreateScreenDepthStencilView();
}

void PixelShader::SetDepthStencilTarget(int num)
{
  _depthStencilView = DX.CreateDepthStencilView(num);
}

void PixelShader::ClearDepthStencilTarget()
{
  _depthStencilView = NULL;
}

void PixelShader::updateNumRenderTargets()
{
  for(int i = MAX_RENDER_TARGETS - 1; i >= 0; i--)
  {
    if (_renderTargets[i] != NULL)
    {
      _numRTSlot = i + 1;
      break;
    }
  }
}

void PixelShader::UseResourceRenderTarget(int resourceSlot, int numTargetInDX)
{
  if (resourceSlot < 0 || resourceSlot >= MAX_RENDER_TARGETS)
    return;

  _shaderResources[resourceSlot] = DX.GetShaderResourceRenderTarget(numTargetInDX);
  updateNumShaderResources();
}

void PixelShader::ClearResourceRenderTarget(int resourceSlot)
{
  if (resourceSlot < 0 || resourceSlot >= MAX_RENDER_TARGETS)
    return;

  if (!_shaderResources[resourceSlot])
    return;

  _shaderResources[resourceSlot] = NULL;
  updateNumShaderResources();
}

void PixelShader::ClearResourcesRenderTarget()
{
  for (int i = 0; i < MAX_SHADER_RESOURCES; i++)
    _shaderResources[i] = NULL;

  _startSRSlot = 0;
  _numSRSlot = 0;
}

void PixelShader::updateNumShaderResources()
{
  for (int i = 0; i < MAX_SHADER_RESOURCES; i++)
  {
    if (_shaderResources[i] != NULL)
    {
      _startSRSlot = i;
      break;
    }
  }

  for(int i = MAX_SHADER_RESOURCES - 1; i >= _startSRSlot; i--)
  {
    if (_shaderResources[i] != NULL)
    {
      _numSRSlot = i - _startSRSlot + 1;
      break;
    }
  }
}

ShaderPass::ShaderPass(UINT orderPass) :
  _orderPass(orderPass),
  _vShader(NULL),
  _pShader(NULL),
  _type(Buffer::BT_NONE)
{
}

ShaderPass::~ShaderPass()
{
}

void ShaderPass::SetVertexShader(const wchar_t* path, VertexShader::TypeVertexShader type, const char* funcName)
{
  _vShader = DX.GetVertexShader(path, type, funcName);

  if (_type != Buffer::BT_NONE)
    setLayout();
}

void ShaderPass::SetVertexShader(VertexShader* vs)
{
  _vShader = vs;
  DX.RegistrationVertexShader(vs);
}

void ShaderPass::SetBufferType(Buffer::BufferType BT_Type)
{
  _type = BT_Type;

  if (_vShader)
    setLayout();
}

void ShaderPass::SetPixelShader(const wchar_t* path, PixelShader::TypePixelShader type, const char* funcName)
{
  _pShader = DX.GetPixelShader(path, type, funcName);
}

void ShaderPass::SetPixelShader(PixelShader* ps)
{
  _pShader = ps;
  DX.RegistrationPixelShader(ps);
}

void ShaderPass::setLayout()
{
  if (!_vShader->CreateLaout(_type))
    _type = Buffer::BT_NONE;
}
