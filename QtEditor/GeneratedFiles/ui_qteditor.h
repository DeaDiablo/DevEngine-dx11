/********************************************************************************
** Form generated from reading UI file 'qteditor.ui'
**
** Created: Sun 26. Aug 13:11:56 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTEDITOR_H
#define UI_QTEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtEditorClass
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *actionExit;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionModel;
    QAction *actionText;
    QAction *actionSprite;
    QAction *actionText_2;
    QAction *actionAbout;
    QAction *actionOptions;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuObject;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtEditorClass)
    {
        if (QtEditorClass->objectName().isEmpty())
            QtEditorClass->setObjectName(QString::fromUtf8("QtEditorClass"));
        QtEditorClass->resize(1113, 836);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QtEditorClass->sizePolicy().hasHeightForWidth());
        QtEditorClass->setSizePolicy(sizePolicy);
        QtEditorClass->setAutoFillBackground(false);
        actionNew = new QAction(QtEditorClass);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionOpen = new QAction(QtEditorClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(QtEditorClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave_as = new QAction(QtEditorClass);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        actionExit = new QAction(QtEditorClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionCopy = new QAction(QtEditorClass);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionPaste = new QAction(QtEditorClass);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionUndo = new QAction(QtEditorClass);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionRedo = new QAction(QtEditorClass);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionCut = new QAction(QtEditorClass);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionModel = new QAction(QtEditorClass);
        actionModel->setObjectName(QString::fromUtf8("actionModel"));
        actionText = new QAction(QtEditorClass);
        actionText->setObjectName(QString::fromUtf8("actionText"));
        actionSprite = new QAction(QtEditorClass);
        actionSprite->setObjectName(QString::fromUtf8("actionSprite"));
        actionText_2 = new QAction(QtEditorClass);
        actionText_2->setObjectName(QString::fromUtf8("actionText_2"));
        actionAbout = new QAction(QtEditorClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionOptions = new QAction(QtEditorClass);
        actionOptions->setObjectName(QString::fromUtf8("actionOptions"));
        centralWidget = new QWidget(QtEditorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QtEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtEditorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1113, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuObject = new QMenu(menuBar);
        menuObject->setObjectName(QString::fromUtf8("menuObject"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        QtEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtEditorClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QtEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtEditorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtEditorClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuObject->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_as);
        menuFile->addSeparator();
        menuFile->addAction(actionOptions);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuObject->addAction(actionModel);
        menuObject->addAction(actionText);
        menuObject->addSeparator();
        menuObject->addAction(actionSprite);
        menuHelp->addAction(actionAbout);

        retranslateUi(QtEditorClass);

        QMetaObject::connectSlotsByName(QtEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtEditorClass)
    {
        QtEditorClass->setWindowTitle(QApplication::translate("QtEditorClass", "QtEditor", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("QtEditorClass", "New", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("QtEditorClass", "Open", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("QtEditorClass", "Save", 0, QApplication::UnicodeUTF8));
        actionSave_as->setText(QApplication::translate("QtEditorClass", "Save as...", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("QtEditorClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("QtEditorClass", "Copy", 0, QApplication::UnicodeUTF8));
        actionPaste->setText(QApplication::translate("QtEditorClass", "Paste", 0, QApplication::UnicodeUTF8));
        actionUndo->setText(QApplication::translate("QtEditorClass", "Undo", 0, QApplication::UnicodeUTF8));
        actionRedo->setText(QApplication::translate("QtEditorClass", "Redo", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("QtEditorClass", "Cut", 0, QApplication::UnicodeUTF8));
        actionModel->setText(QApplication::translate("QtEditorClass", "Model", 0, QApplication::UnicodeUTF8));
        actionText->setText(QApplication::translate("QtEditorClass", "Text", 0, QApplication::UnicodeUTF8));
        actionSprite->setText(QApplication::translate("QtEditorClass", "Sprite", 0, QApplication::UnicodeUTF8));
        actionText_2->setText(QApplication::translate("QtEditorClass", "Text", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("QtEditorClass", "About", 0, QApplication::UnicodeUTF8));
        actionOptions->setText(QApplication::translate("QtEditorClass", "Options", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("QtEditorClass", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("QtEditorClass", "Edit", 0, QApplication::UnicodeUTF8));
        menuObject->setTitle(QApplication::translate("QtEditorClass", "Object", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("QtEditorClass", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QtEditorClass: public Ui_QtEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTEDITOR_H
