/********************************************************************************
** Form generated from reading UI file 'TicketQueryWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TICKETQUERYWIDGET_H
#define UI_TICKETQUERYWIDGET_H

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

class Ui_TicketQueryWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *infoBg;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QLabel *nameLabel;
    QSpacerItem *horizontalSpacer_5;
    QFrame *centerFrame1;
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
    QFrame *centerFrame2;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_7;
    QLabel *detailText;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer_4;
    QTableWidget *tableWidget_2;

    void setupUi(QWidget *TicketQueryWidget)
    {
        if (TicketQueryWidget->objectName().isEmpty())
            TicketQueryWidget->setObjectName(QString::fromUtf8("TicketQueryWidget"));
        TicketQueryWidget->resize(1038, 696);
        horizontalLayout = new QHBoxLayout(TicketQueryWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        infoBg = new QWidget(TicketQueryWidget);
        infoBg->setObjectName(QString::fromUtf8("infoBg"));
        verticalLayout_3 = new QVBoxLayout(infoBg);
        verticalLayout_3->setSpacing(30);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(50, 34, 50, 50);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        nameLabel = new QLabel(infoBg);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(nameLabel);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        verticalLayout_3->addLayout(horizontalLayout_3);

        centerFrame1 = new QFrame(infoBg);
        centerFrame1->setObjectName(QString::fromUtf8("centerFrame1"));
        centerFrame1->setMaximumSize(QSize(16777215, 200));
        centerFrame1->setFrameShape(QFrame::StyledPanel);
        centerFrame1->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(centerFrame1);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        label_3 = new QLabel(centerFrame1);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(6, 32));
        label_3->setMaximumSize(QSize(6, 32));
        label_3->setStyleSheet(QString::fromUtf8("background-color: #009944;\n"
"border-radius: 3px;\n"
""));

        horizontalLayout_2->addWidget(label_3);

        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        basicText = new QLabel(centerFrame1);
        basicText->setObjectName(QString::fromUtf8("basicText"));

        horizontalLayout_2->addWidget(basicText);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        tableWidget = new QTableWidget(centerFrame1);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
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
        tableWidget->setColumnCount(7);

        verticalLayout->addWidget(tableWidget);

        verticalLayout->setStretch(3, 1);

        verticalLayout_3->addWidget(centerFrame1);

        centerFrame2 = new QFrame(infoBg);
        centerFrame2->setObjectName(QString::fromUtf8("centerFrame2"));
        centerFrame2->setFrameShape(QFrame::StyledPanel);
        centerFrame2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(centerFrame2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_6 = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        label_4 = new QLabel(centerFrame2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(6, 32));
        label_4->setMaximumSize(QSize(6, 32));
        label_4->setStyleSheet(QString::fromUtf8("background-color: #009944;\n"
"border-radius: 3px;\n"
""));

        horizontalLayout_4->addWidget(label_4);

        horizontalSpacer_7 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        detailText = new QLabel(centerFrame2);
        detailText->setObjectName(QString::fromUtf8("detailText"));

        horizontalLayout_4->addWidget(detailText);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_4);

        tableWidget_2 = new QTableWidget(centerFrame2);
        if (tableWidget_2->columnCount() < 4)
            tableWidget_2->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(3, __qtablewidgetitem10);
        if (tableWidget_2->rowCount() < 1)
            tableWidget_2->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_2->setItem(0, 0, __qtablewidgetitem11);
        tableWidget_2->setObjectName(QString::fromUtf8("tableWidget_2"));
        sizePolicy.setHeightForWidth(tableWidget_2->sizePolicy().hasHeightForWidth());
        tableWidget_2->setSizePolicy(sizePolicy);
        tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_2->setCornerButtonEnabled(false);
        tableWidget_2->setRowCount(1);
        tableWidget_2->setColumnCount(4);
        tableWidget_2->verticalHeader()->setVisible(false);

        verticalLayout_2->addWidget(tableWidget_2);

        verticalLayout_2->setStretch(3, 1);

        verticalLayout_3->addWidget(centerFrame2);

        verticalLayout_3->setStretch(2, 1);

        horizontalLayout->addWidget(infoBg);


        retranslateUi(TicketQueryWidget);

        QMetaObject::connectSlotsByName(TicketQueryWidget);
    } // setupUi

    void retranslateUi(QWidget *TicketQueryWidget)
    {
        TicketQueryWidget->setWindowTitle(QCoreApplication::translate("TicketQueryWidget", "Form", nullptr));
        nameLabel->setText(QCoreApplication::translate("TicketQueryWidget", "\350\275\246\347\245\250\346\237\245\350\257\242\347\273\223\346\236\234", nullptr));
        label_3->setText(QString());
        basicText->setText(QCoreApplication::translate("TicketQueryWidget", "\350\275\246\347\245\250\345\237\272\346\234\254\344\277\241\346\201\257", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("TicketQueryWidget", "\347\245\250\347\247\215", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("TicketQueryWidget", "\345\215\241\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("TicketQueryWidget", "\347\245\250\345\215\241\346\227\266\351\227\264", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("TicketQueryWidget", "\346\234\211\346\225\210\346\227\245\346\234\237", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("TicketQueryWidget", "\345\215\241\347\212\266\346\200\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("TicketQueryWidget", "\346\227\205\347\250\213\347\212\266\346\200\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("TicketQueryWidget", "\344\275\231\351\242\235", nullptr));
        label_4->setText(QString());
        detailText->setText(QCoreApplication::translate("TicketQueryWidget", "\344\271\230\350\275\246\346\230\216\347\273\206\344\277\241\346\201\257", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("TicketQueryWidget", "\344\272\244\346\230\223\346\227\266\351\227\264", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("TicketQueryWidget", "\344\272\244\346\230\223\347\261\273\345\236\213", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("TicketQueryWidget", "\346\266\210\350\264\271\351\207\221\351\242\235/\346\266\210\350\264\271\346\254\241\346\225\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_2->horizontalHeaderItem(3);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("TicketQueryWidget", "\344\275\231\351\242\235/\345\211\251\344\275\231\346\254\241\346\225\260", nullptr));

        const bool __sortingEnabled = tableWidget_2->isSortingEnabled();
        tableWidget_2->setSortingEnabled(false);
        tableWidget_2->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class TicketQueryWidget: public Ui_TicketQueryWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TICKETQUERYWIDGET_H
