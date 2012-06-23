#ifndef DEV_SHADER_H
#define DEV_SHADER_H

#include <string>

#include <Core/DInclude.h>
#include <DX/DBuffer.h>

namespace dev
{
  class DirectXClass;

  class Shader
  {
  public:
    Shader(const wchar_t* path, DWORD type, const char* functionName);
    virtual ~Shader();

    virtual void SetShader() = 0;
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

    ID3DBlob*               _blob;
    std::wstring            _path;
    DWORD                   _type;
    std::string             _function;
   
    friend DirectXClass;
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

    VertexShader(const wchar_t* path, TypeVertexShader type, const char* fuction);
    virtual ~VertexShader();

    virtual bool CompileShader();
    virtual void SetShader();

  protected:
    virtual const char* getType();
    virtual bool supportTypeShader();

    ID3D11VertexShader* _shader;
  };

  class Layout
  {
  public:
    Layout();
    virtual ~Layout();

    virtual bool CreateLayout(Buffer::BufferType BT_Type, ID3DBlob*& blob);
    virtual void SetLayout();

    inline const Buffer::BufferType& GetType() const
    {
      return _type;
    }

  protected:
    Buffer::BufferType _type;
    ID3D11InputLayout* _layout;
  };

  class PixelShader : public Shader
  {
  public:

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

    PixelShader(const wchar_t* path, TypePixelShader type, const char* fuction);
    virtual ~PixelShader();

    virtual bool CompileShader();
    virtual void SetShader();

  protected:
    virtual const char* getType();
    virtual bool supportTypeShader();

    ID3D11PixelShader* _shader;
  };
}

#endif