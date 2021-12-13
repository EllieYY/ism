/********************************************************************************
** Form generated from reading UI file 'TimeTableWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIMETABLEWIDGET_H
#define UI_TIMETABLEWIDGET_H

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

class Ui_TimeTableWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *subBg;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QLabel *nameLabel;
    QSpacerItem *horizontalSpacer_5;
    QFrame *centerFrame;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_3;
    QLabel *basicText;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QTableWidget *tableWidget;

    void setupUi(QWidget *TimeTableWidget)
    {
        if (TimeTableWidget->objectName().isEmpty())
            TimeTableWidget->setObjectName(QString::fromUtf8("TimeTableWidget"));
        TimeTableWidget->resize(1014, 639);
        horizontalLayout = new QHBoxLayout(TimeTableWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        subBg = new QWidget(TimeTableWidget);
        subBg->setObjectName(QString::fromUtf8("subBg"));
        verticalLayout_2 = new QVBoxLayout(subBg);
        verticalLayout_2->setSpacing(30);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(260, 37, 260, 140);
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


        verticalLayout_2->addLayout(horizontalLayout_3);

        centerFrame = new QFrame(subBg);
        centerFrame->setObjectName(QString::fromUtf8("centerFrame"));
        centerFrame->setFrameShape(QFrame::StyledPanel);
        centerFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(centerFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        label_3 = new QLabel(centerFrame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(6, 32));
        label_3->setMaximumSize(QSize(6, 32));
        label_3->setStyleSheet(QString::fromUtf8("background-color: #009944;\n"
"border-radius: 3px;\n"
""));

        horizontalLayout_2->addWidget(label_3);

        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        basicText = new QLabel(centerFrame);
        basicText->setObjectName(QString::fromUtf8("basicText"));

        horizontalLayout_2->addWidget(basicText);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        tableWidget = new QTableWidget(centerFrame);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        if (tableWidget->rowCount() < 100)
            tableWidget->setRowCount(100);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setFrameShadow(QFrame::Plain);
        tableWidget->setProperty("showDropIndicator", QVariant(true));
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setTextElideMode(Qt::ElideMiddle);
        tableWidget->setGridStyle(Qt::SolidLine);
        tableWidget->setRowCount(100);
        tableWidget->setColumnCount(4);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);

        verticalLayout->addWidget(tableWidget);

        verticalLayout->setStretch(3, 1);

        verticalLayout_2->addWidget(centerFrame);

        verticalLayout_2->setStretch(1, 1);

        horizontalLayout->addWidget(subBg);


        retranslateUi(TimeTableWidget);

        QMetaObject::connectSlotsByName(TimeTableWidget);
    } // setupUi

    void retranslateUi(QWidget *TimeTableWidget)
    {
        TimeTableWidget->setWindowTitle(QCoreApplication::translate("TimeTableWidget", "Form", nullptr));
        nameLabel->setText(QCoreApplication::translate("TimeTableWidget", "\350\277\220\350\220\245\346\227\266\345\210\273\350\241\250", nullptr));
        label_3->setText(QString());
        basicText->setText(QCoreApplication::translate("TimeTableWidget", "\350\277\220\350\220\245\346\227\266\351\227\264", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("TimeTableWidget", "\347\272\277\350\267\257", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("TimeTableWidget", "\350\275\246\347\253\231", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("TimeTableWidget", "\351\246\226\347\217\255\350\275\246", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("TimeTableWidget", "\346\234\253\347\217\255\350\275\246", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TimeTableWidget: public Ui_TimeTableWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIMETABLEWIDGET_H
