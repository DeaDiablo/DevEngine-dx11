#ifndef QTEDITOR_H
#define QTEDITOR_H

#include <QtGui/QMainWindow>
#include "ui_qteditor.h"
#include "qrenderwidget.h"

class QtEditor : public QMainWindow, private Ui::QtEditorClass
{
  Q_OBJECT

public:
  QtEditor(QWidget *parent = 0, Qt::WFlags flags = 0);
  ~QtEditor();

protected:
  QRenderWidget* _renderWidget;

private slots:
  void exit(void);
  void options(void);
};

#endif // QTEDITOR_H
