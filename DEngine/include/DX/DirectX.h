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
  #define DX          DirectX::Instance()
  #define DX_DEVICE   DirectX::GetDevice()
  #define DX_CONTEXT  DirectX::GetContext()

  class DirectX : public Singleton<DirectX>
  {
  //initialize DirectX
  public:
    DirectX();
    virtual ~DirectX();

    bool InitDirectX(HWND hWnd, bool fullScreenMode, UINT msMode, UINT width, UINT height, UINT refreshHz);
    bool CreateDeviceDX();
    bool CreateRasterizerState();
    void CreateViewPort(float width, float height);
    void DestroyDirectX();

    inline bool IsInitialized() const { return _initialized; }
    inline D3D_FEATURE_LEVEL GetFeatureLevel() const { return _featureLevel; }
    inline UINT GetWidth() const { return _width; }
    inline UINT GetHeight() const { return _height; }

    static inline ID3D11Device&         GetDevice()             { return *_dxDevice; }
    static inline ID3D11DeviceContext&  GetContext()            { return *_dxDeviceContext; }
    inline ID3D11RasterizerState*       GetRasterizerState()    { return _dxRasterState; }
  
  protected:
    static ID3D11Device*            _dxDevice;
    static ID3D11DeviceContext*     _dxDeviceContext;
    IDXGISwapChain*          _dxSwapChain;
    ID3D11RasterizerState*   _dxRasterState;
    
    D3D_FEATURE_LEVEL        _featureLevel;
    D3D11_VIEWPORT           _viewPort;

    bool      _initialized;

    HWND      _hWnd;
    UINT      _width, _height;
    UINT      _refreshHz;
    UINT      _msMode;
    bool      _fullScreenMode;

  //Render targets
  public:
    struct RenderTarget
    {
      ID3D11Texture2D*          renderTexture;
      ID3D11RenderTargetView*   renderTargetView;
      ID3D11ShaderResourceView* shaderResource;
    };

    inline ID3D11RenderTargetView** GetActiveRenderTargetView()  
    {
      return _activeRenderTargetView; 
    }

    inline ID3D11Texture2D* GetTextureRenderTarget(int num)
    {
      if (_renderTargets.find(num) != _renderTargets.end())
        return _renderTargets[num]->renderTexture;
      return NULL;
    }

    inline ID3D11ShaderResourceView* GetShaderResourceRenderTarget(int num)
    {
      if (_renderTargets.find(num) != _renderTargets.end())
        return _renderTargets[num]->shaderResource;
      return NULL;
    }

    ID3D11RenderTargetView* CreateScreenRenderTarget();
    ID3D11RenderTargetView* CreateTextureRenderTarget(int targetNum, DXGI_FORMAT format, bool recreate = false);
    void SetRenderTargets(int num, ID3D11RenderTargetView** rtv, ID3D11DepthStencilView* dsv);

  protected:
    void destroyRenderTargets();

    typedef std::map<int, RenderTarget*> RenderTargetMap;
    RenderTargetMap _renderTargets;
    RenderTarget*   _screenRenderTarget;
    ID3D11RenderTargetView** _activeRenderTargetView;
    int _numRTV;
  //Depth and Stencil buffers
  public:
    struct DepthStencilTarget
    {
      ID3D11Texture2D*         depthStencilTexture;
      ID3D11DepthStencilView*  depthStencilView;
    };

    inline ID3D11DepthStencilView* GetActiveDepthStencilView()
    { 
      return _activeDepthStencilView; 
    }
    inline ID3D11DepthStencilState*  GetActiveDepthStencilState()
    { 
      return _activeDepthStencilState; 
    }

    void SetDepthStencilState(ID3D11DepthStencilState* dsState);

    ID3D11DepthStencilView* CreateScreenDepthStencilView();
    ID3D11DepthStencilView* CreateDepthStencilView(int targetNum);
    ID3D11DepthStencilState* CreateDepthStencilState(bool depthEnable = TRUE, D3D11_DEPTH_WRITE_MASK depthMask = D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_FUNC depthComparison = D3D11_COMPARISON_LESS);

  protected:
    DepthStencilTarget* newDepthStencilTarget();
    void destroyDSBuffer();

    typedef std::map<int, DepthStencilTarget*> DepthStencilTargetMap;
    DepthStencilTargetMap _depthStencilTargets;
    DepthStencilTarget* _screenDepthStencilTarget;

    typedef std::set<ID3D11DepthStencilState*> DSStateSet;
    DSStateSet _dsStateVec;

    ID3D11DepthStencilState* _activeDepthStencilState;
    ID3D11DepthStencilView* _activeDepthStencilView;

  //Draw DirectX
  public:
    void ClearAllRenderTargets();
    void ClearAllDepthStencil();
    void ClearActiveRenderTargets();
    void ClearActiveDepthStencil();
    void Present(bool vSync);
  protected:

  //Shader manager DirectX
  public:
    void destroyShaderManager();

    VertexShader* GetVertexShader(const wchar_t* path, VertexShader::TypeVertexShader type, const char* funcName);
    void RegistrationVertexShader(VertexShader* vs);
    PixelShader* GetPixelShader(const wchar_t* path, PixelShader::TypePixelShader type, const char* funcName);
    void RegistrationPixelShader(PixelShader* ps);

  protected:
    typedef std::map<UINT, Shader*> ShaderMap;
    ShaderMap _vertexShaders;
    ShaderMap _pixelShaders;

    typedef std::map<Buffer::BufferType, Shader*> LayoutMap;
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
}

#endif