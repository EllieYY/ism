/********************************************************************************
** Form generated from reading UI file 'TicketTransactionWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TICKETTRANSACTIONWIDGET_H
#define UI_TICKETTRANSACTIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TicketTransactionWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *subBg;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_4;
    QLabel *nameLabel;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QFrame *centerFrame;
    QHBoxLayout *horizontalLayout_5;
    QFrame *sampleFrame;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *stationNameLineEdit;
    QPushButton *selectBtn1;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *payBtn;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_6;

    void setupUi(QWidget *TicketTransactionWidget)
    {
        if (TicketTransactionWidget->objectName().isEmpty())
            TicketTransactionWidget->setObjectName(QString::fromUtf8("TicketTransactionWidget"));
        TicketTransactionWidget->resize(1379, 687);
        horizontalLayout = new QHBoxLayout(TicketTransactionWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        subBg = new QWidget(TicketTransactionWidget);
        subBg->setObjectName(QString::fromUtf8("subBg"));
        subBg->setAutoFillBackground(true);
        verticalLayout_2 = new QVBoxLayout(subBg);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_4 = new QSpacerItem(20, 35, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_4);

        nameLabel = new QLabel(subBg);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(nameLabel);

        verticalSpacer_5 = new QSpacerItem(20, 34, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(260, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        centerFrame = new QFrame(subBg);
        centerFrame->setObjectName(QString::fromUtf8("centerFrame"));
        centerFrame->setFrameShape(QFrame::StyledPanel);
        centerFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(centerFrame);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(92, 0, 123, 0);
        sampleFrame = new QFrame(centerFrame);
        sampleFrame->setObjectName(QString::fromUtf8("sampleFrame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sampleFrame->sizePolicy().hasHeightForWidth());
        sampleFrame->setSizePolicy(sizePolicy);
        sampleFrame->setMinimumSize(QSize(328, 420));
        sampleFrame->setMaximumSize(QSize(328, 420));
        sampleFrame->setFrameShape(QFrame::StyledPanel);
        sampleFrame->setFrameShadow(QFrame::Raised);

        horizontalLayout_5->addWidget(sampleFrame);

        horizontalSpacer_5 = new QSpacerItem(130, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        stationNameLineEdit = new QLineEdit(centerFrame);
        stationNameLineEdit->setObjectName(QString::fromUtf8("stationNameLineEdit"));

        horizontalLayout_2->addWidget(stationNameLineEdit);

        selectBtn1 = new QPushButton(centerFrame);
        selectBtn1->setObjectName(QString::fromUtf8("selectBtn1"));

        horizontalLayout_2->addWidget(selectBtn1);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        payBtn = new QPushButton(centerFrame);
        payBtn->setObjectName(QString::fromUtf8("payBtn"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(payBtn->sizePolicy().hasHeightForWidth());
        payBtn->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(payBtn);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout_5->addLayout(verticalLayout);

        horizontalLayout_5->setStretch(0, 1);

        horizontalLayout_4->addWidget(centerFrame);

        horizontalSpacer_2 = new QSpacerItem(260, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer_6 = new QSpacerItem(20, 120, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_6);

        verticalLayout_2->setStretch(3, 1);

        horizontalLayout->addWidget(subBg);


        retranslateUi(TicketTransactionWidget);

        QMetaObject::connectSlotsByName(TicketTransactionWidget);
    } // setupUi

    void retranslateUi(QWidget *TicketTransactionWidget)
    {
        TicketTransactionWidget->setWindowTitle(QCoreApplication::translate("TicketTransactionWidget", "Form", nullptr));
        nameLabel->setText(QCoreApplication::translate("TicketTransactionWidget", "\350\264\255\347\245\250", nullptr));
        selectBtn1->setText(QCoreApplication::translate("TicketTransactionWidget", "\351\200\211\346\213\251\350\275\246\347\253\231", nullptr));
        payBtn->setText(QCoreApplication::translate("TicketTransactionWidget", "\350\264\255\347\245\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TicketTransactionWidget: public Ui_TicketTransactionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TICKETTRANSACTIONWIDGET_H
