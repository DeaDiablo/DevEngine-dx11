#ifndef QTOPTIONS_H
#define QTOPTIONS_H

#include <QtGui/QWidget>
#include "ui_qtoptions.h"

class QOptions : public QWidget, private Ui::QtOptions
{
  Q_OBJECT

public:
  QOptions(QWidget *parent = 0, Qt::WFlags flags = 0);
  virtual ~QOptions();

};

#endif // QTEDITOR_H
