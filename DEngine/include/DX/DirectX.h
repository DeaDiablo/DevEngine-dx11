#ifndef DEV_DIRECT_X_H
#define DEV_DIRECT_X_H

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
  //********************************************//
  //**************SETUP DIRECTX*****************//
  //********************************************//
  public:
    DirectX();
    virtual ~DirectX();

    bool InitDirectX(HWND hWnd, bool fullScreenMode, UINT msMode, UINT width, UINT height, UINT refreshHz);
    bool CreateDeviceDX();
    void DestroyDirectX();

    inline const bool&                  IsInitialized()   const  { return _initialized;       }
    inline const D3D_FEATURE_LEVEL&     GetFeatureLevel() const  { return _featureLevel;      }
    inline const UINT&                  GetWidth()        const  { return _width;             }
    inline const UINT&                  GetHeight()       const  { return _height;            }
    inline const UINT&                  GetMSMode()       const  { return _msMode;            }

    static inline ID3D11Device&         GetDevice()              { return *_dxDevice;         }
    static inline ID3D11DeviceContext&  GetContext()             { return *_dxDeviceContext;  }

  protected:
    static ID3D11Device*            _dxDevice;
    static ID3D11DeviceContext*     _dxDeviceContext;
    IDXGISwapChain*                 _dxSwapChain;
    
    D3D_FEATURE_LEVEL               _featureLevel;

    bool                            _initialized;

    HWND                            _hWnd;
    UINT                            _width, _height;
    UINT                            _refreshHz;
    UINT                            _msMode;
    bool                            _fullScreenMode;

  //********************************************//
  //****************VIEW PORTS******************//
  //********************************************//
  public:
    void CreateViewPort (float width, float height);
  protected:
    D3D11_VIEWPORT _viewPort;
  //********************************************//
  //**************RENDER TARGETS****************//
  //********************************************//
  public:
    inline ID3D11RenderTargetView* const* GetActiveRenderTargetView()  
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

    ID3D11RenderTargetView*   CreateScreenRenderTarget  ();
    ID3D11RenderTargetView*   CreateTextureRenderTarget (int targetNum, DXGI_FORMAT format, bool recreate = false);
    void                      SetRenderTargets          (int num, ID3D11RenderTargetView* const* rtv, ID3D11DepthStencilView* dsv);

  protected:
    struct RenderTarget
    {
      ID3D11Texture2D*          renderTexture;
      ID3D11RenderTargetView*   renderTargetView;
      ID3D11ShaderResourceView* shaderResource;
    };
    typedef std::map<int, RenderTarget*> RenderTargetMap;
    
    void destroyRenderTargets();

    RenderTargetMap                 _renderTargets;
    RenderTarget*                   _screenRenderTarget;
    ID3D11RenderTargetView* const*  _activeRenderTargetView;
    int                             _numRTV;
  

  //********************************************//
  //*********DEPTH AND STENCIL TARGETS**********//
  //********************************************//
  public:
    inline ID3D11DepthStencilView* GetActiveDepthStencilView()
    { 
      return _activeDepthStencilView; 
    }
    inline ID3D11DepthStencilState*  GetActiveDepthStencilState()
    { 
      return _activeDepthStencilState; 
    }

    inline ID3D11ShaderResourceView* GetShaderResourceScreenDepthStencilTarget()
    {
      if (_screenDepthStencilTarget)
        return _screenDepthStencilTarget->shaderResource;
      return NULL;
    }

    inline ID3D11ShaderResourceView* GetShaderResourceDepthStencilTarget(int num)
    {
      if (_depthStencilTargets.find(num) != _depthStencilTargets.end())
        return _depthStencilTargets[num]->shaderResource;
      return NULL;
    }


    void                      SetDepthStencilState          (ID3D11DepthStencilState* dsState);

    ID3D11DepthStencilView*   CreateScreenDepthStencilView  ();
    ID3D11DepthStencilView*   CreateDepthStencilView        (int targetNum);
    ID3D11DepthStencilState*  CreateDepthStencilState       (bool depthEnable = TRUE, 
                                                             D3D11_DEPTH_WRITE_MASK depthMask = D3D11_DEPTH_WRITE_MASK_ALL, 
                                                             D3D11_COMPARISON_FUNC depthComparison = D3D11_COMPARISON_LESS);

  protected:
    struct DepthStencilTarget
    {
      ID3D11Texture2D*          depthStencilTexture;
      ID3D11DepthStencilView*   depthStencilView;
      ID3D11ShaderResourceView* shaderResource;
    };
    typedef std::map<int, DepthStencilTarget*> DepthStencilTargetMap;
    typedef std::set<ID3D11DepthStencilState*> DSStateSet;

    DepthStencilTarget*       newDepthStencilTarget();
    void                      destroyDSBuffer();

    DepthStencilTargetMap     _depthStencilTargets;
    DepthStencilTarget*       _screenDepthStencilTarget;

    DSStateSet                _dsStateVec;

    ID3D11DepthStencilState*  _activeDepthStencilState;
    ID3D11DepthStencilView*   _activeDepthStencilView;

  //********************************************//
  //***************RASTER STATES****************//
  //********************************************//
  public:
    ID3D11RasterizerState*         CreateRasterizerState    ();
    void                           SetRasterizerState       (ID3D11RasterizerState* rsState);
    inline ID3D11RasterizerState*  GetActiveRasterizerState ()     
    { 
      return _activeRasterState; 
    }
  protected:
    typedef std::set<ID3D11RasterizerState*> RSStateSet;

    void destroyRSStates();
    
    RSStateSet               _rsStateVec;
    ID3D11RasterizerState*   _activeRasterState;
  //********************************************//
  //***************DRAW DIRECTX*****************//
  //********************************************//
  public:
    void ClearAllRenderTargets    ();
    void ClearAllDepthStencil     ();
    void ClearActiveRenderTargets ();
    void ClearActiveDepthStencil  ();
    void Present                  (bool vSync);
  protected:

  //********************************************//
  //******************SHADERS*******************//
  //********************************************//
  public:
    template<class T>
    T* GetShader (const wchar_t* path, DWORD type, const char* funcName)
    {
      UINT hash = getShaderHash(path, type, funcName);
      ShaderMap::iterator i = _shaders.find(hash);
      if (i != _shaders.end())
        return static_cast<T*>((*i).second);

      T* shader = new T(path, type, funcName);
      if (!shader->CompileShader())
      {
        delete shader;
        return NULL;
      }
      _shaders[hash] = shader;
      return shader;
    }

    template<class T>
    void RegistrationShader(T* shader)
    {
      UINT hash = getShaderHash(shader->GetPath(), shader->GetType(), shader->GetFuncName());
      ShaderMap::iterator i = _shaders.find(hash);
      if (i != _shaders.end())
        return;

      if (!shader->IsCompiled())
        shader->CompileShader();
      _shaders[hash] = shader;
    }

    void          VSSetConstantBuffers(UINT startSlot, UINT num, ID3D11Buffer* const* buffer);
    void          PSSetConstantBuffers(UINT startSlot, UINT num, ID3D11Buffer* const* buffer);
  protected:
    typedef std::map<UINT, Shader*> ShaderMap;
    typedef std::map<Buffer::BufferType, Shader*> LayoutMap;

    void destroyShaderManager ();
    UINT getShaderHash        (const wchar_t* path, DWORD type, const char* funcName);

    ShaderMap _shaders;
    LayoutMap _layouts;
  //********************************************//
  //**************TEXTURE MANAGER***************//
  //********************************************//
  public:

    Texture* GetTexture(const wchar_t* path);
    ID3D11SamplerState* GetSamplerState();
    void RemoveTexture(const wchar_t* path);
    void RemoveTexture(Texture* texture);

  protected:
    struct TextureStruct
    {
      Texture* texture;
      int      count;
    };
    typedef std::map<UINT, TextureStruct> TextureMap;
    typedef std::set<ID3D11SamplerState*> SamplerSet;

    void destroyTextureManager();
    UINT getTextureHash       (const wchar_t* path);

    TextureMap _textures;
    SamplerSet _samplers;
  };
}

#endif