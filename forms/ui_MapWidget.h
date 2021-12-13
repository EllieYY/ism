/********************************************************************************
** Form generated from reading UI file 'MapWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPWIDGET_H
#define UI_MAPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *map;

    void setupUi(QWidget *MapWidget)
    {
        if (MapWidget->objectName().isEmpty())
            MapWidget->setObjectName(QString::fromUtf8("MapWidget"));
        MapWidget->resize(770, 469);
        horizontalLayout = new QHBoxLayout(MapWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        map = new QWidget(MapWidget);
        map->setObjectName(QString::fromUtf8("map"));

        horizontalLayout->addWidget(map);


        retranslateUi(MapWidget);

        QMetaObject::connectSlotsByName(MapWidget);
    } // setupUi

    void retranslateUi(QWidget *MapWidget)
    {
        MapWidget->setWindowTitle(QCoreApplication::translate("MapWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MapWidget: public Ui_MapWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPWIDGET_H
