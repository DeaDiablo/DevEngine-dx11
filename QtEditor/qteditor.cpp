#include "qteditor.h"

#include "qtoptions.h"

QtEditor::QtEditor(QWidget *parent, Qt::WFlags flags)
  : QMainWindow(parent, flags)
{
  setupUi(this);
  QObject::connect(actionOptions, SIGNAL(triggered()), this, SLOT(options()));
  QObject::connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));
  
  _renderWidget = new QRenderWidget(this);
  setCentralWidget(_renderWidget);

  //for fullscreen
  //setWindowState(Qt::WindowFullScreen);
  _renderWidget->InitRender();
}

QtEditor::~QtEditor()
{
}


void QtEditor::options(void)
{
  QOptions* optionsWidget = new QOptions(NULL);
  optionsWidget->show();
}

void QtEditor::exit(void)
{
  QApplication::quit();
}