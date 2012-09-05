#include <Model/DDeferredPlane.h>

using namespace dev;

DeferredPlane::DeferredPlane(const wchar_t* name) :
  PlaneIdentity(name)
{
  SetVertexShader(DEFERRED_PASS_NUM, L"plane.fx");
  GetVertexShader(DEFERRED_PASS_NUM)->ClearAllConstantBuffers();

  SetPixelShader(DEFERRED_PASS_NUM, L"plane.fx");
  GetPixelShader(DEFERRED_PASS_NUM)->ClearDepthStencilTarget();
  GetPixelShader(DEFERRED_PASS_NUM)->UseResourceRenderTarget(0, 0);
}

DeferredPlane::~DeferredPlane()
{
}
