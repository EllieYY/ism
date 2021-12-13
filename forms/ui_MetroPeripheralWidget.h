/********************************************************************************
** Form generated from reading UI file 'MetroPeripheralWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_METROPERIPHERALWIDGET_H
#define UI_METROPERIPHERALWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MetroPeripheralWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *metroPer;

    void setupUi(QWidget *MetroPeripheralWidget)
    {
        if (MetroPeripheralWidget->objectName().isEmpty())
            MetroPeripheralWidget->setObjectName(QString::fromUtf8("MetroPeripheralWidget"));
        MetroPeripheralWidget->resize(400, 300);
        horizontalLayout = new QHBoxLayout(MetroPeripheralWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        metroPer = new QWidget(MetroPeripheralWidget);
        metroPer->setObjectName(QString::fromUtf8("metroPer"));

        horizontalLayout->addWidget(metroPer);


        retranslateUi(MetroPeripheralWidget);

        QMetaObject::connectSlotsByName(MetroPeripheralWidget);
    } // setupUi

    void retranslateUi(QWidget *MetroPeripheralWidget)
    {
        MetroPeripheralWidget->setWindowTitle(QCoreApplication::translate("MetroPeripheralWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MetroPeripheralWidget: public Ui_MetroPeripheralWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_METROPERIPHERALWIDGET_H
