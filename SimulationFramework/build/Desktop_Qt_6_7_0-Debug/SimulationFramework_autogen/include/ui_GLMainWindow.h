/********************************************************************************
** Form generated from reading UI file 'GLMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** warOSNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GLMAINWINDOW_H
#define UI_GLMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include "OpenGLWidget.hpp"

QT_BEGIN_NAMESPACE

class Ui_GLMainWindow
{
public:
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionFullScreen;
    QAction *actionFullScreenOpenGL;
    OpenGLWidget *openGLWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuView;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GLMainWindow)
    {
        if (GLMainWindow->objectName().isEmpty())
            GLMainWindow->setObjectName("GLMainWindow");
        GLMainWindow->resize(800, 600);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/galaxy.ico"), QSize(), QIcon::Normal, QIcon::Off);
        GLMainWindow->setWindowIcon(icon);
        actionExit = new QAction(GLMainWindow);
        actionExit->setObjectName("actionExit");
        actionAbout = new QAction(GLMainWindow);
        actionAbout->setObjectName("actionAbout");
        actionFullScreen = new QAction(GLMainWindow);
        actionFullScreen->setObjectName("actionFullScreen");
        actionFullScreen->setCheckable(true);
        actionFullScreenOpenGL = new QAction(GLMainWindow);
        actionFullScreenOpenGL->setObjectName("actionFullScreenOpenGL");
        actionFullScreenOpenGL->setCheckable(true);
        openGLWidget = new OpenGLWidget(GLMainWindow);
        openGLWidget->setObjectName("openGLWidget");
        openGLWidget->setMinimumSize(QSize(128, 128));
        GLMainWindow->setCentralWidget(openGLWidget);
        menuBar = new QMenuBar(GLMainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 800, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName("menuFile");
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName("menuHelp");
        menuView = new QMenu(menuBar);
        menuView->setObjectName("menuView");
        GLMainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(GLMainWindow);
        statusBar->setObjectName("statusBar");
        GLMainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionAbout);
        menuView->addAction(actionFullScreen);
        menuView->addAction(actionFullScreenOpenGL);

        retranslateUi(GLMainWindow);
        QObject::connect(actionExit, &QAction::triggered, GLMainWindow, qOverload<>(&QMainWindow::close));

        QMetaObject::connectSlotsByName(GLMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *GLMainWindow)
    {
        GLMainWindow->setWindowTitle(QCoreApplication::translate("GLMainWindow", "GLMainWindow", nullptr));
        actionExit->setText(QCoreApplication::translate("GLMainWindow", "E&xit", nullptr));
        actionAbout->setText(QCoreApplication::translate("GLMainWindow", "&About", nullptr));
        actionFullScreen->setText(QCoreApplication::translate("GLMainWindow", "&Full Screen", nullptr));
        actionFullScreenOpenGL->setText(QCoreApplication::translate("GLMainWindow", "Full Screen Open&GL", nullptr));
#if QT_CONFIG(shortcut)
        actionFullScreenOpenGL->setShortcut(QCoreApplication::translate("GLMainWindow", "Alt+Return", nullptr));
#endif // QT_CONFIG(shortcut)
        menuFile->setTitle(QCoreApplication::translate("GLMainWindow", "&File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("GLMainWindow", "&Help", nullptr));
        menuView->setTitle(QCoreApplication::translate("GLMainWindow", "&View", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GLMainWindow: public Ui_GLMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GLMAINWINDOW_H
