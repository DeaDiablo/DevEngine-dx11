#include <Core/DRender.h>
#include <Model/DModel.h>
#include <Model/DSkyBox.h>
#include <Model/DDeferredPlane.h>
#include <Element/DCamera.h>
#include <Input/DManipulatorWASD.h>
#include <DX/DConstBuffers.h>

using namespace dev;

void main(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
  Render render(800, 600);
  if (!render.InitRender(800, 600, 60, false))
  {
    MessageBox(0, "Render not init", "Error:", MB_ICONERROR);
    return;
  }
/*

  VertexShader* vs = new VertexShader(L"Tutorial03.fx");
  PixelShader* ps = new PixelShader(L"Tutorial03.fx");
  ps->SetResourceRenderTarget(0, 0, DXGI_FORMAT_R8G8B8A8_UNORM);
  ps->SetResourceRenderTarget(1, 1, DXGI_FORMAT_R16G16B16A16_SNORM);
  ps->SetResourceRenderTarget(2, 2, DXGI_FORMAT_R32G32B32A32_FLOAT);*/


  VertexShader* vs = new VertexShader(L"Tutorial01.fx");
  vs->SetConstantBuffer(2, EYE_POSITION_BUFFER);
  PixelShader* ps = new PixelShader(L"Tutorial011.fx");

  ManipulatorWASD* man = new ManipulatorWASD(L"mans");
  man->SetSpeed(man->GetSpeed()*10);
  Camera* cam = new Camera();
  cam->SetManipulator(man);
  render.RegisterOutputClass(man);
  Scene* scene = new Scene(cam);
  Model* mesh = new Model(L"model.dfm");
  mesh->GetMeshByNum(0)->SetTexture(L"texture.bmp");

  //DeferredPlane* pl = new DeferredPlane(L"Plane");

  SkyBox* sb = new SkyBox(L"skyBox", L"city.dds");
  Group* group = new Group();
  sb->SetCamera(cam);
  scene->AddElement(sb);
  scene->AddElement(group);

  //scene->AddElement(pl, false);

  mesh->SetShaders(0, vs, ps);
  group->AddElement(mesh);

  render.SetScene(scene);
  render.Run();
}