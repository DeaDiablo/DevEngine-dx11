#include <DX/DShader.h>

#include <Core/DLog.h>
#include <DX/DirectX.h>
#include <DX/DConstBuffers.h>

using namespace dev;

Shader::Shader(const wchar_t* path, DWORD type, const char* functionName) :
  _blob(NULL),
  _path(path),
  _type(type),
  _function(functionName),
  _isCompiled(FALSE)
{
  ClearAllConstantBuffers();
}

Shader::~Shader()
{
  releaseBlob();
}

void Shader::SetConstantBuffer(UINT slot, Buffer::ConstantBuffer* constBuffer)
{
  if (slot >= MAX_SHADER_CONST_BUFFERS)
    return;
  
  _constBuffers[slot] = constBuffer->GetBuffer();
  updateNumConstBuffers();
}

void Shader::SetConstantBuffer(UINT slot, Buffer::ConstantBuffer& constBuffer)
{
  if (slot >= MAX_SHADER_CONST_BUFFERS)
    return;

  _constBuffers[slot] = constBuffer.GetBuffer();
  updateNumConstBuffers();
}

void Shader::ClearConstantBuffer(UINT slot)
{
  if (slot >= MAX_SHADER_CONST_BUFFERS || !_constBuffers[slot])
    return;

  _constBuffers[slot] = NULL;
  updateNumConstBuffers();
}

void Shader::ClearAllConstantBuffers()
{
  _startCBSlot = 0;
  _numCBSlot = 0;
  for (UINT i = 0; i < MAX_SHADER_CONST_BUFFERS; i++)
    _constBuffers[i] = NULL;
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

void Shader::updateNumConstBuffers()
{
  for (UINT i = 0; i < MAX_SHADER_CONST_BUFFERS; i++)
  {
    if (_constBuffers[i] != NULL)
    {
      _startCBSlot = i;
      break;
    }
  }

  for(UINT i = MAX_SHADER_CONST_BUFFERS - 1; i >= _startCBSlot; i--)
  {
    if (_constBuffers[i] != NULL)
    {
      _numCBSlot = i - _startCBSlot + 1;
      break;
    }
  }
}

//vertex shader
VertexShader::VertexShader(const wchar_t* path, DWORD type, const char* nameFunction) :
  Shader(path, (DWORD)type, nameFunction),
  _shader(NULL),
  _layout(NULL),
  _bufferType(Buffer::BT_NONE)
{
  SetConstantBuffer(0, VIEW_PROJECTION_BUFFER);
  SetConstantBuffer(1, WORLD_BUFFER);
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

  if (_numCBSlot > 0)
    DX_CONTEXT.VSSetConstantBuffers(_startCBSlot, _numCBSlot, _constBuffers);
  
  DX_CONTEXT.VSSetShader(_shader, NULL, 0);
  DX_CONTEXT.IASetInputLayout(_layout);
  return TRUE;
}

bool VertexShader::CompileShader()
{
  if (!compileShaderFromFile(_path.c_str(), _function.c_str(), getType()))
    return FALSE;

  HRESULT hr = DX_DEVICE.CreateVertexShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), NULL, &_shader);

  if (FAILED(hr))
  {
    releaseBlob();
    WRITE_LOG(L"Vertex shader not created.");
    return FALSE;
  }

  _isCompiled = TRUE;
  return TRUE;
}

bool VertexShader::CreateLayout(Buffer::BufferType BT_Type)
{
  if (!_blob)
    return FALSE;
  
  _bufferType = BT_Type;

  HRESULT hr = DX_DEVICE.CreateInputLayout(Buffer::Declaration::GetLayout(_bufferType), Buffer::Declaration::GetSizeLayout(_bufferType), _blob->GetBufferPointer(), _blob->GetBufferSize(), &_layout);

  if (FAILED(hr))
  {
    WRITE_LOG(L"Layout not created.");
    return FALSE;
  }

  return TRUE;
}


//pixel shader
PixelShader::PixelShader(const wchar_t* path, DWORD type, const char* nameFunction) :
  Shader(path, (DWORD)type, nameFunction),
  _shader(NULL)
{
  ClearAllRenderTargets();
  ClearShaderResources();
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

  if (_numCBSlot > 0)
    DX_CONTEXT.PSSetConstantBuffers(_startCBSlot, _numCBSlot, _constBuffers);
  if (_numRTSlot > 0)
    DX.SetRenderTargets(_numRTSlot, _renderTargets, _depthStencilView);
  if (_numSRSlot > 0)
    DX_CONTEXT.PSSetShaderResources(_startSRSlot, _numSRSlot, _shaderResources);
  DX_CONTEXT.PSSetShader(_shader, NULL, 0);

  return TRUE;
}

bool PixelShader::CompileShader()
{
  if (!compileShaderFromFile(_path.c_str(), _function.c_str(), getType()))
    return FALSE;

  HRESULT hr = DX_DEVICE.CreatePixelShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), NULL, &_shader);
  
  releaseBlob();
  
  if (FAILED(hr))
  {
    WRITE_LOG(L"Pixel shader not created.");
    return FALSE;
  }

  _isCompiled = TRUE;
  return TRUE;
}

void PixelShader::SetScreenRenderTarget(UINT targetSlot)
{
  if (targetSlot < 0 || targetSlot >= MAX_RENDER_TARGETS)
    return;

  _renderTargets[targetSlot] = DX.CreateScreenRenderTarget();
  updateNumRenderTargets();
}

void PixelShader::SetResourceRenderTarget(UINT targetSlot, UINT numTargetInDX, DXGI_FORMAT format)
{
  if (targetSlot < 0 || targetSlot >= MAX_RENDER_TARGETS)
    return;

  _renderTargets[targetSlot] = DX.CreateTextureRenderTarget(numTargetInDX, format);
  updateNumRenderTargets();
}

void PixelShader::ClearRenderTarget(UINT targetSlot)
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
  for (UINT i = 0; i < MAX_RENDER_TARGETS; i++)
    _renderTargets[i] = NULL;
  _numRTSlot = 0;
}

void PixelShader::SetScreenDepthStencil()
{
  _depthStencilView = DX.CreateScreenDepthStencilView();
}

void PixelShader::SetDepthStencilTarget(UINT num)
{
  _depthStencilView = DX.CreateDepthStencilView(num);
}

void PixelShader::ClearDepthStencilTarget()
{
  _depthStencilView = NULL;
}

void PixelShader::updateNumRenderTargets()
{
  for(UINT i = MAX_RENDER_TARGETS - 1; i >= 0; i--)
  {
    if (_renderTargets[i] != NULL)
    {
      _numRTSlot = i + 1;
      break;
    }
  }
}

void PixelShader::UseResourceRenderTarget(UINT resourceSlot, UINT numTargetInDX)
{
  if (resourceSlot < 0 || resourceSlot >= MAX_SHADER_RESOURCES)
    return;

  _shaderResources[resourceSlot] = DX.GetShaderResourceRenderTarget(numTargetInDX);
  updateNumShaderResources();
}

void PixelShader::UseResourceScreenDepthStencilTarget(UINT resourceSlot)
{
  if (resourceSlot < 0 || resourceSlot >= MAX_SHADER_RESOURCES)
    return;

  _shaderResources[resourceSlot] = DX.GetShaderResourceScreenDepthStencilTarget();
  updateNumShaderResources();
}

void PixelShader::UseResourceDepthStencilTarget(UINT resourceSlot, UINT numTargetInDX)
{
  if (resourceSlot < 0 || resourceSlot >= MAX_SHADER_RESOURCES)
    return;

  _shaderResources[resourceSlot] = DX.GetShaderResourceDepthStencilTarget(numTargetInDX);
  updateNumShaderResources();
}

void PixelShader::ClearShaderResource(UINT resourceSlot)
{
  if (resourceSlot < 0 || resourceSlot >= MAX_SHADER_RESOURCES)
    return;

  if (!_shaderResources[resourceSlot])
    return;

  _shaderResources[resourceSlot] = NULL;
  updateNumShaderResources();
}

void PixelShader::ClearShaderResources()
{
  for (UINT i = 0; i < MAX_SHADER_RESOURCES; i++)
    _shaderResources[i] = NULL;

  _startSRSlot = 0;
  _numSRSlot = 0;
}

void PixelShader::updateNumShaderResources()
{
  for (UINT i = 0; i < MAX_SHADER_RESOURCES; i++)
  {
    if (_shaderResources[i] != NULL)
    {
      _startSRSlot = i;
      break;
    }
  }

  for(UINT i = MAX_SHADER_RESOURCES - 1; i >= _startSRSlot; i--)
  {
    if (_shaderResources[i] != NULL)
    {
      _numSRSlot = i - _startSRSlot + 1;
      break;
    }
    if (i == _startSRSlot)
      break;
  }
}


//compute shader
ComputeShader::ComputeShader(const wchar_t* path, DWORD type, const char* nameFunction) :
  Shader(path, (DWORD)type, nameFunction),
  _shader(NULL)
{
}

ComputeShader::~ComputeShader()
{
  if (_shader)
  {
    _shader->Release();
    _shader = NULL;
  }
}

bool ComputeShader::SetShader()
{
  if (!_shader)
    return FALSE;

  DX_CONTEXT.CSSetShader(_shader, NULL, 0);

  return TRUE;
}

bool ComputeShader::CompileShader()
{
  if (!compileShaderFromFile(_path.c_str(), _function.c_str(), getType()))
    return FALSE;

  HRESULT hr = DX_DEVICE.CreateComputeShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), NULL, &_shader);

  releaseBlob();

  if (FAILED(hr))
  {
    WRITE_LOG(L"Pixel shader not created.");
    return FALSE;
  }

  _isCompiled = TRUE;
  return TRUE;
}


const char* ComputeShader::getType()
{
  switch(_type)
  {
  case CS_4_0:
    return "cs_4_0";
  case CS_4_1:
    return "cs_4_1";
  case CS_5_0:
    return "cs_5_0";
  }
  return NULL;
}

bool ComputeShader::supportTypeShader()
{
  switch(_type)
  {
  case CS_4_0:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_0;
  case CS_4_1:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_1;
  case CS_5_0:
    return DX.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0;
  }
  return false;
}
