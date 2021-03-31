/********************************************************************************
** Form generated from reading UI file 'ImageMaster_Pro.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEMASTER_PRO_H
#define UI_IMAGEMASTER_PRO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageMaster_ProClass
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImageMaster_ProClass)
    {
        if (ImageMaster_ProClass->objectName().isEmpty())
            ImageMaster_ProClass->setObjectName(QString::fromUtf8("ImageMaster_ProClass"));
        ImageMaster_ProClass->resize(746, 619);
        centralWidget = new QWidget(ImageMaster_ProClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(240, 80, 160, 80));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(7);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        ImageMaster_ProClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ImageMaster_ProClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 746, 26));
        ImageMaster_ProClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ImageMaster_ProClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ImageMaster_ProClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ImageMaster_ProClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ImageMaster_ProClass->setStatusBar(statusBar);

        retranslateUi(ImageMaster_ProClass);

        QMetaObject::connectSlotsByName(ImageMaster_ProClass);
    } // setupUi

    void retranslateUi(QMainWindow *ImageMaster_ProClass)
    {
        ImageMaster_ProClass->setWindowTitle(QCoreApplication::translate("ImageMaster_ProClass", "ImageMaster_Pro", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImageMaster_ProClass: public Ui_ImageMaster_ProClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEMASTER_PRO_H
