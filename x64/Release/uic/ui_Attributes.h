/********************************************************************************
** Form generated from reading UI file 'Attributes.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATTRIBUTES_H
#define UI_ATTRIBUTES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Attributes
{
public:

    void setupUi(QWidget *Attributes)
    {
        if (Attributes->objectName().isEmpty())
            Attributes->setObjectName(QString::fromUtf8("Attributes"));
        Attributes->resize(400, 300);

        retranslateUi(Attributes);

        QMetaObject::connectSlotsByName(Attributes);
    } // setupUi

    void retranslateUi(QWidget *Attributes)
    {
        Attributes->setWindowTitle(QCoreApplication::translate("Attributes", "Attributes", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Attributes: public Ui_Attributes {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATTRIBUTES_H
