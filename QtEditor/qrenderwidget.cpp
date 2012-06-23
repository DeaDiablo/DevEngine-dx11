#include "qrenderwidget.h"

#include <QtGUI/QMessageBox>

#include <Core/DRender.h>
#include <Model/DModel.h>
#include <Model/DSkyBox.h>
#include <Element/DCamera.h>
#include <Input/DManipulatorWASD.h>

using namespace dev;

QRenderWidget::QRenderWidget(QWidget *parent) : 
  QWidget(parent),
  _initRender(FALSE),
  _render(NULL)
{
  resize(parent->width(), parent->height());
  
  setAttribute(Qt::WA_PaintOnScreen);
  setAttribute(Qt::WA_NativeWindow);
}

QRenderWidget::~QRenderWidget()
{
  if (_render)
  {
    _render->Release();
    _render = NULL;
  }
}

bool QRenderWidget::InitRender(int width, int height, int RefreshHz, bool FullScreenMode)
{
  if (FullScreenMode)
    setWindowState(Qt::WindowFullScreen);
  _render = new Render(winId());
  if (!_render->InitRender(width, height, RefreshHz, FullScreenMode))
  {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText("Error init DirectX!");
    msgBox.exec();
    return FALSE;
  }
  _initRender = TRUE;
  
  ManipulatorWASD* man = new ManipulatorWASD(L"mans");
  Camera* cam = new Camera();
  cam->SetManipulator(man);
  _render->RegisterOutputClass(man);
  Scene* scene = new Scene(cam);
  Model* mesh = new Model(L"model.dfm");
  mesh->SetVertexShader(L"Tutorial03.fx");
  mesh->SetPixelShader(L"Tutorial03.fx");
  mesh->GetMeshByNum(0)->SetTexture(L"texture.bmp");

  SkyBox* sb = new SkyBox(L"skyBox", L"city.dds");
  sb->SetCamera(cam);

  scene->AddElement(sb);
  scene->AddElement(mesh);
  _render->SetScene(scene);
  _render->RunThread();
  return TRUE;
}

void QRenderWidget::paintEvent(QPaintEvent* evt) 
{
  Q_UNUSED(evt);
}

void QRenderWidget::resizeEvent(QResizeEvent* evt) 
{
  /*releaseBuffers();
  swapChain_->ResizeBuffers(1, width(), height(),
    swapChainDesc_.BufferDesc.Format, 0);
  swapChain_->GetDesc(&swapChainDesc_);
  viewport_.Width = width();
  viewport_.Height = height();
  createBuffers();*/
}
