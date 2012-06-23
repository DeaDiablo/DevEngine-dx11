#ifndef DEV_WIDGET_H
#define DEV_WIDGET_H

#include <QtGui/QWidget>
#include <Core/DRender.h>

class QRenderWidget : public QWidget
{
  Q_OBJECT
  Q_DISABLE_COPY(QRenderWidget)

public:
  QRenderWidget(QWidget* parent = NULL);
  virtual ~QRenderWidget();
  bool InitRender(int width = 0, int height = 0, int RefreshHz = 0, bool FullScreenMode = false);
protected:
  virtual void resizeEvent(QResizeEvent* evt);
  virtual void paintEvent(QPaintEvent* evt);
  dev::Render* _render;
  bool _initRender;
};

#endif