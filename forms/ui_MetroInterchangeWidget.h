/********************************************************************************
** Form generated from reading UI file 'MetroInterchangeWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_METROINTERCHANGEWIDGET_H
#define UI_METROINTERCHANGEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MetroInterchangeWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *subBg;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QLabel *nameLabel;
    QSpacerItem *horizontalSpacer_5;
    QFrame *centerFrame;
    QHBoxLayout *horizontalLayout_2;
    QTableWidget *tableWidget;

    void setupUi(QWidget *MetroInterchangeWidget)
    {
        if (MetroInterchangeWidget->objectName().isEmpty())
            MetroInterchangeWidget->setObjectName(QString::fromUtf8("MetroInterchangeWidget"));
        MetroInterchangeWidget->resize(912, 571);
        horizontalLayout = new QHBoxLayout(MetroInterchangeWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        subBg = new QWidget(MetroInterchangeWidget);
        subBg->setObjectName(QString::fromUtf8("subBg"));
        verticalLayout = new QVBoxLayout(subBg);
        verticalLayout->setSpacing(30);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(260, 37, 260, 140);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        nameLabel = new QLabel(subBg);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(nameLabel);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_3);

        centerFrame = new QFrame(subBg);
        centerFrame->setObjectName(QString::fromUtf8("centerFrame"));
        centerFrame->setFrameShape(QFrame::StyledPanel);
        centerFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(centerFrame);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        tableWidget = new QTableWidget(centerFrame);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (tableWidget->rowCount() < 1)
            tableWidget->setRowCount(1);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setRowCount(1);
        tableWidget->setColumnCount(3);

        horizontalLayout_2->addWidget(tableWidget);


        verticalLayout->addWidget(centerFrame);

        verticalLayout->setStretch(1, 1);

        horizontalLayout->addWidget(subBg);


        retranslateUi(MetroInterchangeWidget);

        QMetaObject::connectSlotsByName(MetroInterchangeWidget);
    } // setupUi

    void retranslateUi(QWidget *MetroInterchangeWidget)
    {
        MetroInterchangeWidget->setWindowTitle(QCoreApplication::translate("MetroInterchangeWidget", "Form", nullptr));
        nameLabel->setText(QCoreApplication::translate("MetroInterchangeWidget", "\346\215\242\344\271\230\346\237\245\350\257\242", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MetroInterchangeWidget", "\347\272\277\350\267\257", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MetroInterchangeWidget", "\350\275\246\347\253\231", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MetroInterchangeWidget", "\346\215\242\344\271\230\350\267\257\347\272\277", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MetroInterchangeWidget: public Ui_MetroInterchangeWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_METROINTERCHANGEWIDGET_H
