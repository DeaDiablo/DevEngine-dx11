#include <Model/DDeferredPlane.h>

using namespace dev;

DeferredPlane::DeferredPlane(const wchar_t* name) :
  PlaneIdentity(name)
{
  SetVertexShader(DEFERRED_PASS_NUM, L"shaders/deferredPlane.vs");
  GetVertexShader(DEFERRED_PASS_NUM)->ClearAllConstantBuffers();

  SetPixelShader(DEFERRED_PASS_NUM, L"shaders/deferredPlane.ps");
  GetPixelShader(DEFERRED_PASS_NUM)->ClearDepthStencilTarget();
  GetPixelShader(DEFERRED_PASS_NUM)->UseResourceRenderTarget(0, 0);
  GetPixelShader(DEFERRED_PASS_NUM)->UseResourceRenderTarget(1, 1);
}

DeferredPlane::~DeferredPlane()
{
}
