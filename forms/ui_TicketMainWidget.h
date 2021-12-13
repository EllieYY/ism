/********************************************************************************
** Form generated from reading UI file 'TicketMainWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TICKETMAINWIDGET_H
#define UI_TICKETMAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TicketMainWidget
{
public:
    QHBoxLayout *horizontalLayout_3;
    QWidget *mainBg;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *bigNameLabel;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QToolButton *purchaseBtn;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *refundBtn;
    QSpacerItem *horizontalSpacer_4;
    QToolButton *queryBtn;
    QSpacerItem *horizontalSpacer_5;
    QToolButton *reregisterBtn;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *TicketMainWidget)
    {
        if (TicketMainWidget->objectName().isEmpty())
            TicketMainWidget->setObjectName(QString::fromUtf8("TicketMainWidget"));
        TicketMainWidget->resize(911, 571);
        horizontalLayout_3 = new QHBoxLayout(TicketMainWidget);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        mainBg = new QWidget(TicketMainWidget);
        mainBg->setObjectName(QString::fromUtf8("mainBg"));
        mainBg->setAutoFillBackground(true);
        verticalLayout = new QVBoxLayout(mainBg);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 81, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        bigNameLabel = new QLabel(mainBg);
        bigNameLabel->setObjectName(QString::fromUtf8("bigNameLabel"));
        bigNameLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(bigNameLabel);

        verticalSpacer_2 = new QSpacerItem(20, 67, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        purchaseBtn = new QToolButton(mainBg);
        purchaseBtn->setObjectName(QString::fromUtf8("purchaseBtn"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(purchaseBtn->sizePolicy().hasHeightForWidth());
        purchaseBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(purchaseBtn);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        refundBtn = new QToolButton(mainBg);
        refundBtn->setObjectName(QString::fromUtf8("refundBtn"));
        sizePolicy.setHeightForWidth(refundBtn->sizePolicy().hasHeightForWidth());
        refundBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(refundBtn);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        queryBtn = new QToolButton(mainBg);
        queryBtn->setObjectName(QString::fromUtf8("queryBtn"));
        sizePolicy.setHeightForWidth(queryBtn->sizePolicy().hasHeightForWidth());
        queryBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(queryBtn);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        reregisterBtn = new QToolButton(mainBg);
        reregisterBtn->setObjectName(QString::fromUtf8("reregisterBtn"));
        sizePolicy.setHeightForWidth(reregisterBtn->sizePolicy().hasHeightForWidth());
        reregisterBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(reregisterBtn);

        horizontalLayout->setStretch(0, 31);
        horizontalLayout->setStretch(1, 5);
        horizontalLayout->setStretch(2, 31);
        horizontalLayout->setStretch(3, 5);
        horizontalLayout->setStretch(4, 31);
        horizontalLayout->setStretch(5, 5);
        horizontalLayout->setStretch(6, 31);

        horizontalLayout_2->addLayout(horizontalLayout);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        horizontalLayout_2->setStretch(0, 85);
        horizontalLayout_2->setStretch(1, 910);
        horizontalLayout_2->setStretch(2, 85);

        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_3 = new QSpacerItem(20, 79, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        verticalLayout->setStretch(0, 130);
        verticalLayout->setStretch(1, 96);
        verticalLayout->setStretch(2, 108);
        verticalLayout->setStretch(3, 420);
        verticalLayout->setStretch(4, 128);

        horizontalLayout_3->addWidget(mainBg);


        retranslateUi(TicketMainWidget);

        QMetaObject::connectSlotsByName(TicketMainWidget);
    } // setupUi

    void retranslateUi(QWidget *TicketMainWidget)
    {
        TicketMainWidget->setWindowTitle(QCoreApplication::translate("TicketMainWidget", "Form", nullptr));
        bigNameLabel->setText(QCoreApplication::translate("TicketMainWidget", "\347\245\250\345\215\241\345\244\204\347\220\206", nullptr));
        purchaseBtn->setText(QCoreApplication::translate("TicketMainWidget", "...", nullptr));
        refundBtn->setText(QCoreApplication::translate("TicketMainWidget", "...", nullptr));
        queryBtn->setText(QCoreApplication::translate("TicketMainWidget", "...", nullptr));
        reregisterBtn->setText(QCoreApplication::translate("TicketMainWidget", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TicketMainWidget: public Ui_TicketMainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TICKETMAINWIDGET_H
