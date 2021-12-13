/********************************************************************************
** Form generated from reading UI file 'PaymentWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAYMENTWIDGET_H
#define UI_PAYMENTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PaymentWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *subBg;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_4;
    QLabel *nameLabel;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QFrame *centerFrame;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_3;
    QLabel *text9;
    QSpacerItem *verticalSpacer_2;
    QGridLayout *gridLayout;
    QPushButton *aliPayBtn;
    QPushButton *wePayBtn;
    QPushButton *cashBtn;
    QSpacerItem *horizontalSpacer_2;
    QLabel *aliPayLabel;
    QLabel *wePayLabel;
    QLabel *cashLabel;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *updateBtn;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *okBtn;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_6;

    void setupUi(QWidget *PaymentWidget)
    {
        if (PaymentWidget->objectName().isEmpty())
            PaymentWidget->setObjectName(QString::fromUtf8("PaymentWidget"));
        PaymentWidget->resize(1323, 1056);
        horizontalLayout = new QHBoxLayout(PaymentWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        subBg = new QWidget(PaymentWidget);
        subBg->setObjectName(QString::fromUtf8("subBg"));
        subBg->setAutoFillBackground(true);
        verticalLayout_5 = new QVBoxLayout(subBg);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_4 = new QSpacerItem(20, 34, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_5->addItem(verticalSpacer_4);

        nameLabel = new QLabel(subBg);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(nameLabel);

        verticalSpacer_5 = new QSpacerItem(20, 35, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_5->addItem(verticalSpacer_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(260, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        centerFrame = new QFrame(subBg);
        centerFrame->setObjectName(QString::fromUtf8("centerFrame"));
        centerFrame->setFrameShape(QFrame::StyledPanel);
        centerFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(centerFrame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(70, 0, -1, 50);
        verticalSpacer_3 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);

        text9 = new QLabel(centerFrame);
        text9->setObjectName(QString::fromUtf8("text9"));

        verticalLayout->addWidget(text9);

        verticalSpacer_2 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(70);
        gridLayout->setVerticalSpacing(30);
        aliPayBtn = new QPushButton(centerFrame);
        aliPayBtn->setObjectName(QString::fromUtf8("aliPayBtn"));
        aliPayBtn->setMinimumSize(QSize(150, 150));
        aliPayBtn->setMaximumSize(QSize(150, 150));

        gridLayout->addWidget(aliPayBtn, 0, 0, 1, 1);

        wePayBtn = new QPushButton(centerFrame);
        wePayBtn->setObjectName(QString::fromUtf8("wePayBtn"));
        wePayBtn->setMinimumSize(QSize(150, 150));
        wePayBtn->setMaximumSize(QSize(150, 150));

        gridLayout->addWidget(wePayBtn, 0, 1, 1, 1);

        cashBtn = new QPushButton(centerFrame);
        cashBtn->setObjectName(QString::fromUtf8("cashBtn"));
        cashBtn->setMinimumSize(QSize(150, 150));
        cashBtn->setMaximumSize(QSize(150, 150));

        gridLayout->addWidget(cashBtn, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 3, 1, 1);

        aliPayLabel = new QLabel(centerFrame);
        aliPayLabel->setObjectName(QString::fromUtf8("aliPayLabel"));
        aliPayLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(aliPayLabel, 1, 0, 1, 1);

        wePayLabel = new QLabel(centerFrame);
        wePayLabel->setObjectName(QString::fromUtf8("wePayLabel"));
        wePayLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(wePayLabel, 1, 1, 1, 1);

        cashLabel = new QLabel(centerFrame);
        cashLabel->setObjectName(QString::fromUtf8("cashLabel"));
        cashLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(cashLabel, 1, 2, 1, 1);

        gridLayout->setRowStretch(0, 1);
        gridLayout->setColumnStretch(3, 1);

        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 487, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        updateBtn = new QPushButton(centerFrame);
        updateBtn->setObjectName(QString::fromUtf8("updateBtn"));

        horizontalLayout_3->addWidget(updateBtn);

        horizontalSpacer_4 = new QSpacerItem(60, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        okBtn = new QPushButton(centerFrame);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));

        horizontalLayout_3->addWidget(okBtn);

        horizontalSpacer_3 = new QSpacerItem(130, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalLayout->setStretch(4, 1);

        horizontalLayout_4->addWidget(centerFrame);

        horizontalSpacer_6 = new QSpacerItem(260, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        horizontalLayout_4->setStretch(1, 1);

        verticalLayout_5->addLayout(horizontalLayout_4);

        verticalSpacer_6 = new QSpacerItem(20, 135, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_5->addItem(verticalSpacer_6);

        verticalLayout_5->setStretch(3, 1);

        horizontalLayout->addWidget(subBg);


        retranslateUi(PaymentWidget);

        QMetaObject::connectSlotsByName(PaymentWidget);
    } // setupUi

    void retranslateUi(QWidget *PaymentWidget)
    {
        PaymentWidget->setWindowTitle(QCoreApplication::translate("PaymentWidget", "Form", nullptr));
        nameLabel->setText(QCoreApplication::translate("PaymentWidget", "\344\273\230\346\254\276", nullptr));
        text9->setText(QCoreApplication::translate("PaymentWidget", "\350\257\267\347\241\256\350\256\244\344\273\230\346\254\276\346\226\271\345\274\217\345\271\266\347\202\271\345\207\273\347\241\256\350\256\244\346\214\211\351\222\256", nullptr));
        aliPayBtn->setText(QString());
        wePayBtn->setText(QString());
        cashBtn->setText(QString());
        aliPayLabel->setText(QCoreApplication::translate("PaymentWidget", "\346\224\257\344\273\230\345\256\235", nullptr));
        wePayLabel->setText(QCoreApplication::translate("PaymentWidget", "\345\276\256\344\277\241", nullptr));
        cashLabel->setText(QCoreApplication::translate("PaymentWidget", "\347\216\260\351\207\221", nullptr));
        updateBtn->setText(QCoreApplication::translate("PaymentWidget", "\345\210\267\346\226\260", nullptr));
        okBtn->setText(QCoreApplication::translate("PaymentWidget", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PaymentWidget: public Ui_PaymentWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAYMENTWIDGET_H
