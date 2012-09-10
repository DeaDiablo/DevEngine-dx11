#include <Model/DDeferredPlane.h>

using namespace dev;

DeferredPlane::DeferredPlane(const wchar_t* name) :
  PlaneIdentity(name)
{
  SetVertexShader(DEFERRED_PASS_NUM, L"shaders/deferredPlane.vs");
  VertexShader* vs = GetVertexShader(DEFERRED_PASS_NUM);
  if (vs)
  {
    vs->ClearAllConstantBuffers();
  }

  SetPixelShader(DEFERRED_PASS_NUM, L"shaders/deferredPlane.ps");
  PixelShader* ps = GetPixelShader(DEFERRED_PASS_NUM);
  if (ps)
  {
    ps->ClearDepthStencilTarget();
    ps->UseResourceRenderTarget(0, 0);
    ps->UseResourceRenderTarget(1, 1);
    ps->UseResourceRenderTarget(2, 2);
    ps->SetConstantBuffer(0, EYE_POSITION_BUFFER);
  }
}

DeferredPlane::~DeferredPlane()
{
}
