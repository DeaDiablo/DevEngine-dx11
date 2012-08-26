/********************************************************************************
** Form generated from reading UI file 'qtoptions.ui'
**
** Created: Sun 26. Aug 13:11:56 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTOPTIONS_H
#define UI_QTOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtOptions
{
public:
    QComboBox *comboBox;
    QSplitter *splitter;
    QPushButton *OK;
    QPushButton *Cancel;

    void setupUi(QWidget *QtOptions)
    {
        if (QtOptions->objectName().isEmpty())
            QtOptions->setObjectName(QString::fromUtf8("QtOptions"));
        QtOptions->resize(270, 109);
        comboBox = new QComboBox(QtOptions);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(50, 10, 181, 22));
        splitter = new QSplitter(QtOptions);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(20, 70, 225, 23));
        splitter->setOrientation(Qt::Horizontal);
        OK = new QPushButton(splitter);
        OK->setObjectName(QString::fromUtf8("OK"));
        splitter->addWidget(OK);
        Cancel = new QPushButton(splitter);
        Cancel->setObjectName(QString::fromUtf8("Cancel"));
        splitter->addWidget(Cancel);
        QWidget::setTabOrder(OK, Cancel);

        retranslateUi(QtOptions);

        QMetaObject::connectSlotsByName(QtOptions);
    } // setupUi

    void retranslateUi(QWidget *QtOptions)
    {
        QtOptions->setWindowTitle(QApplication::translate("QtOptions", "Options", 0, QApplication::UnicodeUTF8));
        OK->setText(QApplication::translate("QtOptions", "OK", 0, QApplication::UnicodeUTF8));
        Cancel->setText(QApplication::translate("QtOptions", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QtOptions: public Ui_QtOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTOPTIONS_H
