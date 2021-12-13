/********************************************************************************
** Form generated from reading UI file 'ISMFrame.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ISMFRAME_H
#define UI_ISMFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>

QT_BEGIN_NAMESPACE

class Ui_ISMFrame
{
public:

    void setupUi(QFrame *ISMFrame)
    {
        if (ISMFrame->objectName().isEmpty())
            ISMFrame->setObjectName(QString::fromUtf8("ISMFrame"));
        ISMFrame->resize(821, 582);

        retranslateUi(ISMFrame);

        QMetaObject::connectSlotsByName(ISMFrame);
    } // setupUi

    void retranslateUi(QFrame *ISMFrame)
    {
        ISMFrame->setWindowTitle(QCoreApplication::translate("ISMFrame", "Frame", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ISMFrame: public Ui_ISMFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ISMFRAME_H
