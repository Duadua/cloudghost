/********************************************************************************
** Form generated from reading UI file 'cpbr.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CPBR_H
#define UI_CPBR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CPBRClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CPBRClass)
    {
        if (CPBRClass->objectName().isEmpty())
            CPBRClass->setObjectName(QString::fromUtf8("CPBRClass"));
        CPBRClass->resize(600, 400);
        centralWidget = new QWidget(CPBRClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        CPBRClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CPBRClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 29));
        CPBRClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CPBRClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        CPBRClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CPBRClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        CPBRClass->setStatusBar(statusBar);

        retranslateUi(CPBRClass);

        QMetaObject::connectSlotsByName(CPBRClass);
    } // setupUi

    void retranslateUi(QMainWindow *CPBRClass)
    {
        CPBRClass->setWindowTitle(QApplication::translate("CPBRClass", "CPBR", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CPBRClass: public Ui_CPBRClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CPBR_H
