/********************************************************************************
** Form generated from reading UI file 'RefundWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REFUNDWIDGET_H
#define UI_REFUNDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RefundWidget
{
public:
    QVBoxLayout *verticalLayout_4;
    QWidget *infoBg;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_4;
    QLabel *nameLabel;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_2;
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
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_6;
    QFrame *refundSample;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *sRefundBtn;
    QSpacerItem *horizontalSpacer_8;

    void setupUi(QWidget *RefundWidget)
    {
        if (RefundWidget->objectName().isEmpty())
            RefundWidget->setObjectName(QString::fromUtf8("RefundWidget"));
        RefundWidget->resize(1025, 687);
        verticalLayout_4 = new QVBoxLayout(RefundWidget);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        infoBg = new QWidget(RefundWidget);
        infoBg->setObjectName(QString::fromUtf8("infoBg"));
        verticalLayout_3 = new QVBoxLayout(infoBg);
        verticalLayout_3->setSpacing(30);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(50, 34, 50, 50);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        nameLabel = new QLabel(infoBg);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(nameLabel);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);


        verticalLayout_3->addLayout(horizontalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(20);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        centerFrame1 = new QFrame(infoBg);
        centerFrame1->setObjectName(QString::fromUtf8("centerFrame1"));
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
        tableWidget->setMaximumSize(QSize(16777215, 120));
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setRowCount(1);
        tableWidget->setColumnCount(7);

        verticalLayout->addWidget(tableWidget);

        verticalLayout->setStretch(3, 1);

        verticalLayout_2->addWidget(centerFrame1);

        centerFrame2 = new QFrame(infoBg);
        centerFrame2->setObjectName(QString::fromUtf8("centerFrame2"));
        centerFrame2->setFrameShape(QFrame::StyledPanel);
        centerFrame2->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(centerFrame2);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 55, 0, 21);
        horizontalSpacer_6 = new QSpacerItem(191, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        refundSample = new QFrame(centerFrame2);
        refundSample->setObjectName(QString::fromUtf8("refundSample"));
        sizePolicy.setHeightForWidth(refundSample->sizePolicy().hasHeightForWidth());
        refundSample->setSizePolicy(sizePolicy);
        refundSample->setFrameShape(QFrame::StyledPanel);
        refundSample->setFrameShadow(QFrame::Raised);

        horizontalLayout_3->addWidget(refundSample);

        horizontalSpacer_7 = new QSpacerItem(209, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        sRefundBtn = new QPushButton(centerFrame2);
        sRefundBtn->setObjectName(QString::fromUtf8("sRefundBtn"));

        horizontalLayout_3->addWidget(sRefundBtn);

        horizontalSpacer_8 = new QSpacerItem(208, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_8);


        verticalLayout_2->addWidget(centerFrame2);

        verticalLayout_2->setStretch(1, 1);

        verticalLayout_3->addLayout(verticalLayout_2);


        verticalLayout_4->addWidget(infoBg);


        retranslateUi(RefundWidget);

        QMetaObject::connectSlotsByName(RefundWidget);
    } // setupUi

    void retranslateUi(QWidget *RefundWidget)
    {
        RefundWidget->setWindowTitle(QCoreApplication::translate("RefundWidget", "Form", nullptr));
        nameLabel->setText(QCoreApplication::translate("RefundWidget", "\351\200\200\346\254\276", nullptr));
        label_3->setText(QString());
        basicText->setText(QCoreApplication::translate("RefundWidget", "\350\275\246\347\245\250\345\237\272\346\234\254\344\277\241\346\201\257", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("RefundWidget", "\347\245\250\347\247\215", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("RefundWidget", "\345\215\241\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("RefundWidget", "\347\245\250\345\215\241\346\227\266\351\227\264", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("RefundWidget", "\346\234\211\346\225\210\346\227\245\346\234\237", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("RefundWidget", "\345\215\241\347\212\266\346\200\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("RefundWidget", "\346\227\205\347\250\213\347\212\266\346\200\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("RefundWidget", "\344\275\231\351\242\235", nullptr));
        sRefundBtn->setText(QCoreApplication::translate("RefundWidget", "\351\200\200\346\254\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RefundWidget: public Ui_RefundWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REFUNDWIDGET_H
