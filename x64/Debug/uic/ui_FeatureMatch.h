/********************************************************************************
** Form generated from reading UI file 'FeatureMatch.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FEATUREMATCH_H
#define UI_FEATUREMATCH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FeatureMatch
{
public:

    void setupUi(QWidget *FeatureMatch)
    {
        if (FeatureMatch->objectName().isEmpty())
            FeatureMatch->setObjectName(QString::fromUtf8("FeatureMatch"));
        FeatureMatch->resize(400, 300);

        retranslateUi(FeatureMatch);

        QMetaObject::connectSlotsByName(FeatureMatch);
    } // setupUi

    void retranslateUi(QWidget *FeatureMatch)
    {
        FeatureMatch->setWindowTitle(QCoreApplication::translate("FeatureMatch", "FeatureMatch", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FeatureMatch: public Ui_FeatureMatch {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FEATUREMATCH_H
