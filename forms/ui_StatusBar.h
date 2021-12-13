/********************************************************************************
** Form generated from reading UI file 'StatusBar.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATUSBAR_H
#define UI_STATUSBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatusBar
{
public:
    QHBoxLayout *horizontalLayout_2;
    QWidget *statusBg;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *netStateLabel;
    QSpacerItem *horizontalSpacer_2;
    QLabel *siteNameLabel;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *homeBtn;
    QPushButton *returnBtn;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *StatusBar)
    {
        if (StatusBar->objectName().isEmpty())
            StatusBar->setObjectName(QString::fromUtf8("StatusBar"));
        StatusBar->resize(959, 88);
        StatusBar->setMinimumSize(QSize(0, 88));
        StatusBar->setMaximumSize(QSize(16777215, 88));
        horizontalLayout_2 = new QHBoxLayout(StatusBar);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        statusBg = new QWidget(StatusBar);
        statusBg->setObjectName(QString::fromUtf8("statusBg"));
        horizontalLayout = new QHBoxLayout(statusBg);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        netStateLabel = new QLabel(statusBg);
        netStateLabel->setObjectName(QString::fromUtf8("netStateLabel"));

        horizontalLayout->addWidget(netStateLabel);

        horizontalSpacer_2 = new QSpacerItem(200, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        siteNameLabel = new QLabel(statusBg);
        siteNameLabel->setObjectName(QString::fromUtf8("siteNameLabel"));
        siteNameLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(siteNameLabel);

        horizontalSpacer_3 = new QSpacerItem(186, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        homeBtn = new QPushButton(statusBg);
        homeBtn->setObjectName(QString::fromUtf8("homeBtn"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(homeBtn->sizePolicy().hasHeightForWidth());
        homeBtn->setSizePolicy(sizePolicy);
        homeBtn->setMinimumSize(QSize(177, 0));
        homeBtn->setMaximumSize(QSize(177, 16777215));

        horizontalLayout->addWidget(homeBtn);

        returnBtn = new QPushButton(statusBg);
        returnBtn->setObjectName(QString::fromUtf8("returnBtn"));
        sizePolicy.setHeightForWidth(returnBtn->sizePolicy().hasHeightForWidth());
        returnBtn->setSizePolicy(sizePolicy);
        returnBtn->setMinimumSize(QSize(177, 0));
        returnBtn->setMaximumSize(QSize(177, 16777215));

        horizontalLayout->addWidget(returnBtn);

        horizontalSpacer_4 = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        horizontalLayout->setStretch(2, 1);
        horizontalLayout->setStretch(4, 1);

        horizontalLayout_2->addWidget(statusBg);


        retranslateUi(StatusBar);

        QMetaObject::connectSlotsByName(StatusBar);
    } // setupUi

    void retranslateUi(QWidget *StatusBar)
    {
        StatusBar->setWindowTitle(QCoreApplication::translate("StatusBar", "Form", nullptr));
        netStateLabel->setText(QCoreApplication::translate("StatusBar", "TextLabel", nullptr));
        siteNameLabel->setText(QCoreApplication::translate("StatusBar", "\345\215\227\346\230\214\345\244\247\346\241\245\344\270\234\347\253\231", nullptr));
        homeBtn->setText(QString());
        returnBtn->setText(QCoreApplication::translate("StatusBar", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StatusBar: public Ui_StatusBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATUSBAR_H
