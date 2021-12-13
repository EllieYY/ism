/********************************************************************************
** Form generated from reading UI file 'StationListWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATIONLISTWIDGET_H
#define UI_STATIONLISTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StationListWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;

    void setupUi(QWidget *StationListWidget)
    {
        if (StationListWidget->objectName().isEmpty())
            StationListWidget->setObjectName(QString::fromUtf8("StationListWidget"));
        StationListWidget->resize(450, 382);
        horizontalLayout = new QHBoxLayout(StationListWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(StationListWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        horizontalLayout->addWidget(listWidget);


        retranslateUi(StationListWidget);

        QMetaObject::connectSlotsByName(StationListWidget);
    } // setupUi

    void retranslateUi(QWidget *StationListWidget)
    {
        StationListWidget->setWindowTitle(QCoreApplication::translate("StationListWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StationListWidget: public Ui_StationListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATIONLISTWIDGET_H
