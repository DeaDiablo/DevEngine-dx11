#ifndef DEV_SHADER_H
#define DEV_SHADER_H

#include <map>

#include <Core/DInclude.h>
#include <Core/DSettings.h>
#include <DX/DBuffer.h>

namespace dev
{
  class VertexShader;
  class PixelShader;

  class Shader
  {
  public:
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

    void Release()
    {
      delete this;
    }

    bool IsCompiled() 
    {
      return _isCompiled;
    }

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

    ID3DBlob*       _blob;
    std::wstring    _path;
    DWORD           _type;
    std::string     _function;
    bool            _isCompiled;
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

    VertexShader(const wchar_t* path, TypeVertexShader type = VS_4_0, const char* nameFunction = DEFAULT_VS_FUNCTION);
    virtual ~VertexShader();

    virtual bool CompileShader();
    virtual bool CreateLaout(Buffer::BufferType BT_Type);
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
    #define MAX_SHADER_RESOURCES  D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT

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

    PixelShader(const wchar_t* path, TypePixelShader type = PS_4_0, const char* nameFunction = DEFAULT_PS_FUNCTION);
    virtual ~PixelShader();

    virtual bool CompileShader();
    virtual bool SetShader();
   
    virtual void SetScreenRenderTarget(int targetSlot);
    virtual void SetResourceRenderTarget(int targetSlot, int numTargetInDX, DXGI_FORMAT format);
    virtual void ClearRenderTarget(int targetSlot);
    virtual void ClearAllRenderTargets();
    
    virtual void SetScreenDepthStencil();
    virtual void SetDepthStencilTarget(int num);
    virtual void ClearDepthStencilTarget();

    virtual void UseResourceRenderTarget(int resourceSlot, int numTargetInDX);
    virtual void ClearResourceRenderTarget(int resourceSlot);
    virtual void ClearResourcesRenderTarget();

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
    int _numRTSlot;
    ID3D11ShaderResourceView* _shaderResources[MAX_SHADER_RESOURCES];
    int _startSRSlot, _numSRSlot;
  };

  class ShaderPass
  {
  public:
    ShaderPass(UINT orderPass);
    virtual ~ShaderPass();

    void SetVertexShader(const wchar_t* path, VertexShader::TypeVertexShader type, const char* funcName);
    void SetVertexShader(VertexShader* vs);
    inline VertexShader* GetVertexShader() 
    { 
      return _vShader; 
    }

    void SetBufferType(Buffer::BufferType BT_Type);
    inline const Buffer::BufferType& GetType() const
    {
      return _type;
    }

    void SetPixelShader(const wchar_t* path, PixelShader::TypePixelShader type, const char* funcName);
    void SetPixelShader(PixelShader* ps);
    inline PixelShader* GetPixelShader() 
    { 
      return _pShader; 
    }

    inline UINT GetOrderPass() const
    {
      return _orderPass;
    }

  protected:
    void setLayout();
    UINT _orderPass;

    Buffer::BufferType _type;
    VertexShader* _vShader;
    PixelShader*  _pShader;
  };

  typedef std::map<UINT, ShaderPass*> ShaderPassMap;
}

#endif