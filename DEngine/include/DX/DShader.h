#ifndef DEV_SHADER_H
#define DEV_SHADER_H

#include <Core/DInclude.h>
#include <Core/DSettings.h>
#include <DX/DBuffer.h>

namespace dev
{
  class VertexShader;
  class PixelShader;
  class ComputeShader;

  class Shader
  {
  public:

    #define MAX_SHADER_RESOURCES       D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT
    #define MAX_SHADER_CONST_BUFFERS   D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT

    Shader(const wchar_t* path, DWORD type, const char* functionName);
    virtual ~Shader();

    virtual bool SetShader() = 0;
    virtual bool CompileShader() = 0;

    inline const wchar_t* GetPath() const
    {
      return _path.c_str();
    }

    inline const DWORD& GetType() const
    {
      return _type;
    }

    inline const char* GetFuncName() const
    {
      return _function.c_str();
    }

    virtual VertexShader* AsVertexShader() 
    { 
      return NULL; 
    }

    virtual PixelShader* AsPixelShader() 
    { 
      return NULL; 
    }

    virtual ComputeShader* AsComputeShader()
    {
      return NULL;
    }

    void Release()
    {
      delete this;
    }

    bool IsCompiled() 
    {
      return _isCompiled;
    }

    virtual void SetConstantBuffer(UINT slot, Buffer::ConstantBuffer* constBuffer);
    virtual void SetConstantBuffer(UINT slot, Buffer::ConstantBuffer& constBuffer);
    virtual void ClearConstantBuffer(UINT slot);
    virtual void ClearAllConstantBuffers();

  protected:

    inline void releaseBlob() 
    {
      if (_blob)
      {
        _blob->Release();
        _blob = NULL;
      }
    }

    virtual const char* getType() = 0;
    virtual bool supportTypeShader() = 0;

    bool compileShaderFromFile(const wchar_t* fileName, const char* functionName, const char* shaderModel);
    virtual void updateNumConstBuffers();

    ID3DBlob*       _blob;
    std::wstring    _path;
    DWORD           _type;
    std::string     _function;
    bool            _isCompiled;
    
    UINT _startCBSlot, _numCBSlot;
    ID3D11Buffer* _constBuffers[MAX_SHADER_CONST_BUFFERS];
  };

  class VertexShader : public Shader
  {
  public:

    enum TypeVertexShader
    {
      VS_1_1,
      VS_2_0,
      VS_2_A,
      VS_2_SW,
      VS_3_0,
      VS_3_SW,
      VS_4_0,
      VS_4_1,
      VS_5_0
    };

    VertexShader(const wchar_t* path, DWORD type = VS_4_0, const char* nameFunction = DEFAULT_VS_FUNCTION);
    virtual ~VertexShader();

    virtual bool CompileShader();
    virtual bool CreateLayout(Buffer::BufferType BT_Type);
    virtual bool SetShader();

    inline const Buffer::BufferType& GetBufferType() const
    {
      return _bufferType;
    }

    virtual VertexShader* AsVertexShader() 
    { 
      return this; 
    }

  protected:
    virtual const char* getType();
    virtual bool supportTypeShader();

    Buffer::BufferType    _bufferType;
    ID3D11VertexShader*   _shader;
    ID3D11InputLayout*    _layout;
  };

  class PixelShader : public Shader
  {
  public:

    #define MAX_RENDER_TARGETS    D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT

    enum TypePixelShader
    {
      PS_2_0,
      PS_2_A,
      PS_2_B,
      PS_2_SW,
      PS_3_0,
      PS_3_SW,
      PS_4_0,
      PS_4_1,
      PS_5_0
    };

    PixelShader(const wchar_t* path, DWORD type = PS_4_0, const char* nameFunction = DEFAULT_PS_FUNCTION);
    virtual ~PixelShader();

    virtual bool CompileShader();
    virtual bool SetShader();
   
    virtual void SetScreenRenderTarget(UINT targetSlot);
    virtual void SetResourceRenderTarget(UINT targetSlot, UINT numTargetInDX, DXGI_FORMAT format);
    virtual void ClearRenderTarget(UINT targetSlot);
    virtual void ClearAllRenderTargets();
    
    virtual void SetScreenDepthStencil();
    virtual void SetDepthStencilTarget(UINT num);
    virtual void ClearDepthStencilTarget();

    virtual void UseResourceRenderTarget(UINT resourceSlot, UINT numTargetInDX);
    virtual void UseResourceScreenDepthStencilTarget(UINT resourceSlot);
    virtual void UseResourceDepthStencilTarget(UINT resourceSlot, UINT numTargetInDX);
    virtual void ClearShaderResource(UINT resourceSlot);
    virtual void ClearShaderResources();

    virtual PixelShader* AsPixelShader() 
    { 
      return this; 
    }

  protected:
    virtual void updateNumRenderTargets();
    virtual void updateNumShaderResources();
    virtual const char* getType();
    virtual bool supportTypeShader();

    ID3D11PixelShader* _shader;
    ID3D11DepthStencilView* _depthStencilView;
    ID3D11RenderTargetView* _renderTargets[MAX_RENDER_TARGETS];
    UINT _numRTSlot;
    ID3D11ShaderResourceView* _shaderResources[MAX_SHADER_RESOURCES];
    UINT _startSRSlot, _numSRSlot;
  };

  class ComputeShader : public Shader
  {
  public:

    enum TypeComputeShader
    {
      CS_4_0,
      CS_4_1,
      CS_5_0
    };

    ComputeShader(const wchar_t* path, DWORD type = CS_5_0, const char* nameFunction = DEFAULT_CS_FUNCTION);
    virtual ~ComputeShader();

    virtual bool CompileShader();
    virtual bool SetShader();

    virtual ComputeShader* AsComputeShader() 
    { 
      return this; 
    }

  protected:
    virtual const char* getType();
    virtual bool supportTypeShader();

    ID3D11ComputeShader* _shader;
  };

  struct ShaderStruct
  {
    ShaderStruct() : vs(NULL), ps(NULL), cs(NULL) {}
    VertexShader*  vs;
    ComputeShader* cs;
    PixelShader*   ps;
  };
}

#endif