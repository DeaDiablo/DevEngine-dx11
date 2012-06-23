#ifndef DEV_TEXTURE_H
#define DEV_TEXTURE_H

#include <string>
#include <vector>

#include <Core/DInclude.h>

#define MAX_NUM_TEXTURES 8

namespace dev
{
  //base class texture
  class Texture
  {
  public:

    enum TypeTexture
    {
      TEX_2D,
      TEX_CUBE
    };

    Texture(const wchar_t* path);
    virtual ~Texture();

    const wchar_t* GetPath() const
    {
      return _path.c_str();
    }

    virtual void SetVSTexture(int num);
    virtual void SetPSTexture(int num);

  protected:
    virtual void destroy();

    std::wstring _path;
    ID3D11ShaderResourceView* _texture;
  };

  typedef std::vector<Texture*> TextureList;
}

#endif