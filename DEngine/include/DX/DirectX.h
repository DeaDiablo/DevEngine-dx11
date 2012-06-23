#ifndef DEV_DIRECT_X_H
#define DEV_DIRECT_X_H

#include <set>
#include <map>

#include <DX/DShader.h>
#include <DX/DTexture.h>
#include <Core/DInclude.h>
#include <Core/DSingleton.h>

namespace dev
{
  #define DX          DirectX::Get()
  #define DX_DEVICE   DirectX::Get()->GetDevice()
  #define DX_CONTEXT  DirectX::Get()->GetContext()

  class DirectXClass
  {
  //initialize DirectX
  public:
    DirectXClass();
    virtual ~DirectXClass();

    bool InitDirectX(HWND hWnd, bool fullScreenMode, UINT msMode, UINT width, UINT height, UINT refreshHz);
    bool CreateDeviceDX();
    bool CreateRenderTargetDX();
    bool CreateRasterizerState();
    void CreateViewPort(float width, float height);
    void DestroyDirectX();

    inline bool IsInitialized() const { return _initialized; }
    inline D3D_FEATURE_LEVEL GetFeatureLevel() const { return _featureLevel; }
    inline UINT GetWidth() const { return _width; }
    inline UINT GetHeight() const { return _height; }

    inline ID3D11Device*             GetDevice()             { return _dxDevice; }
    inline ID3D11DeviceContext*      GetContext()            { return _dxDeviceContext; }
    inline ID3D11RasterizerState*    GetRasterizerState()    { return _dxRasterState; }
  
  protected:
    ID3D11Device*            _dxDevice;
    ID3D11DeviceContext*     _dxDeviceContext;
    IDXGISwapChain*          _dxSwapChain;
    ID3D11RenderTargetView*  _dxRenderTargetView;
    ID3D11RasterizerState*   _dxRasterState;
    
    D3D_FEATURE_LEVEL        _featureLevel;
    D3D11_VIEWPORT           _viewPort;

    bool      _initialized;

    HWND      _hWnd;
    UINT      _width, _height;
    UINT      _refreshHz;
    UINT      _msMode;
    bool      _fullScreenMode;

  //Depth and Stencil buffers
  public:
    void ClearDSBuffer();
    ID3D11DepthStencilView*   GetDepthStencilView()   { return _dxDepthStencilView; }
    ID3D11DepthStencilState*  GetDepthStencilState()  { return _dxDepthStencilState; }

    void SetDepthStencilView(ID3D11DepthStencilView* dsView);
    void SetDepthStencilState(ID3D11DepthStencilState* dsState);

    ID3D11DepthStencilView* CreateDepthStencilView();
    ID3D11DepthStencilState* CreateDepthStencilState(bool depthEnable = TRUE, D3D11_DEPTH_WRITE_MASK depthMask = D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_FUNC depthComparison = D3D11_COMPARISON_LESS);

  protected:
    typedef std::set<ID3D11Texture2D*> DSTex2DSet;
    DSTex2DSet _dsTex2DVec;

    typedef std::set<ID3D11DepthStencilView*> DSViewSet;
    DSViewSet _dsViewVec;

    typedef std::set<ID3D11DepthStencilState*> DSStateSet;
    DSStateSet _dsStateVec;

    ID3D11DepthStencilView*  _dxDepthStencilView;
    ID3D11DepthStencilState* _dxDepthStencilState;

  //Draw DirectX
  public:
    void ClearFrame();
    void Present(bool vSync);
  protected:

  //Shader manager DirectX
  public:
    void destroyShaderManager();

    VertexShader* GetVertexShader(const wchar_t* path, VertexShader::TypeVertexShader type, const char* funcName);
    Layout* GetLayout(Buffer::BufferType BT_Type, VertexShader* vShader);
    PixelShader* GetPixelShader(const wchar_t* path, PixelShader::TypePixelShader type, const char* funcName);

    void RemoveVertexShader(VertexShader* shader);
    void RemoveLayout(Layout* layout);
    void RemovePixelShader(PixelShader* shader);

  protected:
    struct ShaderStruct
    {
      union
      {
        VertexShader* vertexShader;
        PixelShader* pixelShader;
        Layout* layout;
      };
      UINT count;
    };

    ShaderStruct* _buffer;

    typedef std::map<UINT, ShaderStruct*> ShaderMap;
    ShaderMap _vertexShaders;
    ShaderMap _pixelShaders;

    typedef std::map<Buffer::BufferType, ShaderStruct*> LayoutMap;
    LayoutMap _layouts;

    UINT getShaderHash(const wchar_t* path, DWORD type, const char* funcName);

  //Texture manager DirectX
  public:
    void destroyTextureManager();

    Texture* GetTexture(const wchar_t* path);
    ID3D11SamplerState* GetSamplerState();
    void RemoveTexture(const wchar_t* path);
    void RemoveTexture(Texture* texture);

  protected:
    void clearTextureList();

    struct TextureStruct
    {
      Texture* texture;
      int      count;
    };

    UINT getTextureHash(const wchar_t* path);

    typedef std::map<UINT, TextureStruct> TextureMap;
    TextureMap _textures;

    typedef std::set<ID3D11SamplerState*> SamplerSet;
    SamplerSet _samplers;
  };

  class DirectX : public Singleton<DirectXClass>
  {};
}

#endif