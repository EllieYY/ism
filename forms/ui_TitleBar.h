/********************************************************************************
** Form generated from reading UI file 'TitleBar.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TITLEBAR_H
#define UI_TITLEBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TitleBar
{
public:
    QHBoxLayout *horizontalLayout_2;
    QWidget *titleBg;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *iconLabel;
    QSpacerItem *horizontalSpacer_3;
    QLabel *stateLabel;
    QSpacerItem *horizontalSpacer_4;
    QLabel *dateLabel;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *TitleBar)
    {
        if (TitleBar->objectName().isEmpty())
            TitleBar->setObjectName(QString::fromUtf8("TitleBar"));
        TitleBar->resize(918, 110);
        TitleBar->setMinimumSize(QSize(0, 110));
        TitleBar->setMaximumSize(QSize(16777215, 110));
        TitleBar->setAutoFillBackground(true);
        horizontalLayout_2 = new QHBoxLayout(TitleBar);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        titleBg = new QWidget(TitleBar);
        titleBg->setObjectName(QString::fromUtf8("titleBg"));
        horizontalLayout = new QHBoxLayout(titleBg);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(22, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        iconLabel = new QLabel(titleBg);
        iconLabel->setObjectName(QString::fromUtf8("iconLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(iconLabel->sizePolicy().hasHeightForWidth());
        iconLabel->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(iconLabel);

        horizontalSpacer_3 = new QSpacerItem(323, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        stateLabel = new QLabel(titleBg);
        stateLabel->setObjectName(QString::fromUtf8("stateLabel"));
        stateLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(stateLabel);

        horizontalSpacer_4 = new QSpacerItem(210, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        dateLabel = new QLabel(titleBg);
        dateLabel->setObjectName(QString::fromUtf8("dateLabel"));
        dateLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(dateLabel);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        horizontalLayout->setStretch(0, 40);
        horizontalLayout->setStretch(1, 291);
        horizontalLayout->setStretch(2, 531);
        horizontalLayout->setStretch(3, 196);
        horizontalLayout->setStretch(4, 348);
        horizontalLayout->setStretch(5, 465);
        horizontalLayout->setStretch(6, 49);

        horizontalLayout_2->addWidget(titleBg);


        retranslateUi(TitleBar);

        QMetaObject::connectSlotsByName(TitleBar);
    } // setupUi

    void retranslateUi(QWidget *TitleBar)
    {
        TitleBar->setWindowTitle(QCoreApplication::translate("TitleBar", "Form", nullptr));
        iconLabel->setText(QString());
        stateLabel->setText(QCoreApplication::translate("TitleBar", "TextLabel", nullptr));
        dateLabel->setText(QCoreApplication::translate("TitleBar", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TitleBar: public Ui_TitleBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TITLEBAR_H
