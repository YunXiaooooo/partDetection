/********************************************************************************
** Form generated from reading UI file 'project.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECT_H
#define UI_PROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_projectClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *projectClass)
    {
        if (projectClass->objectName().isEmpty())
            projectClass->setObjectName(QString::fromUtf8("projectClass"));
        projectClass->resize(600, 400);
        menuBar = new QMenuBar(projectClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        projectClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(projectClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        projectClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(projectClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        projectClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(projectClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        projectClass->setStatusBar(statusBar);

        retranslateUi(projectClass);

        QMetaObject::connectSlotsByName(projectClass);
    } // setupUi

    void retranslateUi(QMainWindow *projectClass)
    {
        projectClass->setWindowTitle(QApplication::translate("projectClass", "project", nullptr));
    } // retranslateUi

};

namespace Ui {
    class projectClass: public Ui_projectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECT_H
