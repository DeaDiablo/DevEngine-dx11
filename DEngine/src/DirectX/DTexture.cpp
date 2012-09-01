#include <DX/DTexture.h>

#include <Core/DLog.h>
#include <DX/DirectX.h>

using namespace dev;

Texture::Texture(const wchar_t* path) :
  _path(path),
  _texture(NULL)
{
  if(FAILED(D3DX11CreateShaderResourceViewFromFileW(&DX_DEVICE, path, NULL, NULL, &_texture, NULL )))
  {
    std::wstring buf(L"Texture not loaded: ");
    buf += path;
    WRITE_LOG(buf.c_str());
    destroy();
  }
}

Texture::~Texture()
{
  destroy();
}

void Texture::destroy()
{
  _path.clear();
  if (_texture)
  {
    _texture->Release();
    _texture = NULL;
  }
}

void Texture::SetVSTexture(int num)
{
  if (num < MAX_NUM_TEXTURES && _texture)
    DX_CONTEXT.VSSetShaderResources(0, num + 1, &_texture);
}

void Texture::SetPSTexture(int num)
{
  if (num < MAX_NUM_TEXTURES && _texture)
    DX_CONTEXT.PSSetShaderResources(0, num + 1, &_texture);
}