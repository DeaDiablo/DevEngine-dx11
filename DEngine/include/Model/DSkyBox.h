#ifndef DEV_SKY_BOX_H
#define DEV_SKY_BOX_H

#include <Element/DCamera.h>
#include <Model/DMesh.h>

namespace dev
{
  class SkyBox : public Mesh
  {
  public:
    SkyBox(const wchar_t* name, const wchar_t* texture);
    virtual ~SkyBox();

    void SetCamera(Camera* camera);

    virtual void Update(const Matrix& matrix);
    virtual void updateParameters();
    virtual void returnParameters();
  protected:
    Camera* _camera;
    ID3D11DepthStencilState* _skyDSState;
    ID3D11DepthStencilState* _oldDSState;
  };
}

#endif