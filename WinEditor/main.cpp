#include <Core/DRender.h>
#include <Model/DModel.h>
#include <Model/DSkyBox.h>
#include <Element/DCamera.h>
#include <Input/DManipulatorWASD.h>

using namespace dev;

void main(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
  Render* render = new Render(800, 600);
  if (!render->InitRender(800, 600, 60, false))
  {
    MessageBox(0, "Render not init", "Error:", MB_ICONERROR);
    return;
  }

  ManipulatorWASD* man = new ManipulatorWASD(L"mans");
  Camera* cam = new Camera();
  cam->SetManipulator(man);
  render->RegisterOutputClass(man);
  Scene* scene = new Scene(cam);
  Model* mesh = new Model(L"model.dfm");
  mesh->SetVertexShader(L"Tutorial03.fx");
  mesh->SetPixelShader(L"Tutorial03.fx");
  mesh->GetMeshByNum(0)->SetTexture(L"texture.bmp");

  SkyBox* sb = new SkyBox(L"skyBox", L"city.dds");
  sb->SetCamera(cam);

  scene->AddElement(sb);
  scene->AddElement(mesh);
  render->SetScene(scene);
  render->Run();

  render->Release();
}