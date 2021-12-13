/********************************************************************************
** Form generated from reading UI file 'MainWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWidget
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
    QToolButton *cardBtn;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *QRCodeBtn;
    QSpacerItem *horizontalSpacer_4;
    QToolButton *inquiryBtn;
    QSpacerItem *horizontalSpacer_5;
    QToolButton *infoBtn;
    QSpacerItem *horizontalSpacer_6;
    QToolButton *guidBtn;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *MainWidget)
    {
        if (MainWidget->objectName().isEmpty())
            MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        MainWidget->resize(1914, 568);
        horizontalLayout_3 = new QHBoxLayout(MainWidget);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        mainBg = new QWidget(MainWidget);
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
        horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cardBtn = new QToolButton(mainBg);
        cardBtn->setObjectName(QString::fromUtf8("cardBtn"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cardBtn->sizePolicy().hasHeightForWidth());
        cardBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(cardBtn);

        horizontalSpacer_3 = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        QRCodeBtn = new QToolButton(mainBg);
        QRCodeBtn->setObjectName(QString::fromUtf8("QRCodeBtn"));
        sizePolicy.setHeightForWidth(QRCodeBtn->sizePolicy().hasHeightForWidth());
        QRCodeBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(QRCodeBtn);

        horizontalSpacer_4 = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        inquiryBtn = new QToolButton(mainBg);
        inquiryBtn->setObjectName(QString::fromUtf8("inquiryBtn"));
        sizePolicy.setHeightForWidth(inquiryBtn->sizePolicy().hasHeightForWidth());
        inquiryBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(inquiryBtn);

        horizontalSpacer_5 = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        infoBtn = new QToolButton(mainBg);
        infoBtn->setObjectName(QString::fromUtf8("infoBtn"));
        sizePolicy.setHeightForWidth(infoBtn->sizePolicy().hasHeightForWidth());
        infoBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(infoBtn);

        horizontalSpacer_6 = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        guidBtn = new QToolButton(mainBg);
        guidBtn->setObjectName(QString::fromUtf8("guidBtn"));
        sizePolicy.setHeightForWidth(guidBtn->sizePolicy().hasHeightForWidth());
        guidBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(guidBtn);


        horizontalLayout_2->addLayout(horizontalLayout);

        horizontalSpacer_2 = new QSpacerItem(80, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        horizontalLayout_2->setStretch(0, 85);
        horizontalLayout_2->setStretch(1, 910);
        horizontalLayout_2->setStretch(2, 85);

        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_3 = new QSpacerItem(20, 79, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout_3->addWidget(mainBg);


        retranslateUi(MainWidget);

        QMetaObject::connectSlotsByName(MainWidget);
    } // setupUi

    void retranslateUi(QWidget *MainWidget)
    {
        MainWidget->setWindowTitle(QCoreApplication::translate("MainWidget", "Form", nullptr));
        bigNameLabel->setText(QCoreApplication::translate("MainWidget", "\346\254\242\350\277\216\344\275\277\347\224\250\346\231\272\350\203\275\345\256\242\346\234\215", nullptr));
        cardBtn->setText(QCoreApplication::translate("MainWidget", "...", nullptr));
        QRCodeBtn->setText(QCoreApplication::translate("MainWidget", "...", nullptr));
        inquiryBtn->setText(QCoreApplication::translate("MainWidget", "...", nullptr));
        infoBtn->setText(QCoreApplication::translate("MainWidget", "...", nullptr));
        guidBtn->setText(QCoreApplication::translate("MainWidget", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWidget: public Ui_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H
