/********************************************************************************
** Form generated from reading UI file 'TicketPriceWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TICKETPRICEWIDGET_H
#define UI_TICKETPRICEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TicketPriceWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *infoBg;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_6;
    QLabel *nameLabel;
    QSpacerItem *horizontalSpacer_7;
    QFrame *centerFrame;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_3;
    QGridLayout *gridLayout;
    QLabel *text1;
    QLineEdit *lineEdit;
    QPushButton *selectBtn2;
    QLabel *text2;
    QLineEdit *lineEdit_2;
    QPushButton *selectBtn3;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *priceCalBtn;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *verticalSpacer_7;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *priceText;
    QLineEdit *lineEdit_3;
    QLabel *priceUnit;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_4;
    QFrame *priceSample;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *horizontalSpacer_5;

    void setupUi(QWidget *TicketPriceWidget)
    {
        if (TicketPriceWidget->objectName().isEmpty())
            TicketPriceWidget->setObjectName(QString::fromUtf8("TicketPriceWidget"));
        TicketPriceWidget->resize(1248, 687);
        horizontalLayout = new QHBoxLayout(TicketPriceWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        infoBg = new QWidget(TicketPriceWidget);
        infoBg->setObjectName(QString::fromUtf8("infoBg"));
        verticalLayout_3 = new QVBoxLayout(infoBg);
        verticalLayout_3->setSpacing(34);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(120, 36, 120, 50);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        nameLabel = new QLabel(infoBg);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(nameLabel);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);


        verticalLayout_3->addLayout(horizontalLayout_4);

        centerFrame = new QFrame(infoBg);
        centerFrame->setObjectName(QString::fromUtf8("centerFrame"));
        centerFrame->setFrameShape(QFrame::StyledPanel);
        centerFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(centerFrame);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(120, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_3 = new QSpacerItem(20, 110, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(38);
        gridLayout->setVerticalSpacing(40);
        text1 = new QLabel(centerFrame);
        text1->setObjectName(QString::fromUtf8("text1"));
        text1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text1, 0, 0, 1, 1);

        lineEdit = new QLineEdit(centerFrame);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 60));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        selectBtn2 = new QPushButton(centerFrame);
        selectBtn2->setObjectName(QString::fromUtf8("selectBtn2"));

        gridLayout->addWidget(selectBtn2, 0, 2, 1, 1);

        text2 = new QLabel(centerFrame);
        text2->setObjectName(QString::fromUtf8("text2"));
        text2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text2, 1, 0, 1, 1);

        lineEdit_2 = new QLineEdit(centerFrame);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setMinimumSize(QSize(0, 60));

        gridLayout->addWidget(lineEdit_2, 1, 1, 1, 1);

        selectBtn3 = new QPushButton(centerFrame);
        selectBtn3->setObjectName(QString::fromUtf8("selectBtn3"));

        gridLayout->addWidget(selectBtn3, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer_2 = new QSpacerItem(20, 70, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        priceCalBtn = new QPushButton(centerFrame);
        priceCalBtn->setObjectName(QString::fromUtf8("priceCalBtn"));

        horizontalLayout_5->addWidget(priceCalBtn);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_9);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_7);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(28);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        priceText = new QLabel(centerFrame);
        priceText->setObjectName(QString::fromUtf8("priceText"));
        priceText->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(priceText);

        lineEdit_3 = new QLineEdit(centerFrame);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setMinimumSize(QSize(0, 60));
        lineEdit_3->setMaximumSize(QSize(120, 60));

        horizontalLayout_2->addWidget(lineEdit_3);

        priceUnit = new QLabel(centerFrame);
        priceUnit->setObjectName(QString::fromUtf8("priceUnit"));
        priceUnit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(priceUnit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        verticalLayout->setStretch(6, 1);

        horizontalLayout_3->addLayout(verticalLayout);

        horizontalSpacer_4 = new QSpacerItem(160, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);

        priceSample = new QFrame(centerFrame);
        priceSample->setObjectName(QString::fromUtf8("priceSample"));
        priceSample->setMinimumSize(QSize(362, 391));
        priceSample->setMaximumSize(QSize(362, 391));
        priceSample->setFrameShape(QFrame::StyledPanel);
        priceSample->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(priceSample);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_5);

        verticalLayout_2->setStretch(1, 1);

        horizontalLayout_3->addLayout(verticalLayout_2);

        horizontalSpacer_5 = new QSpacerItem(43, 80, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        verticalLayout_3->addWidget(centerFrame);


        horizontalLayout->addWidget(infoBg);


        retranslateUi(TicketPriceWidget);

        QMetaObject::connectSlotsByName(TicketPriceWidget);
    } // setupUi

    void retranslateUi(QWidget *TicketPriceWidget)
    {
        TicketPriceWidget->setWindowTitle(QCoreApplication::translate("TicketPriceWidget", "Form", nullptr));
        nameLabel->setText(QCoreApplication::translate("TicketPriceWidget", "\347\245\250\344\273\267\346\237\245\350\257\242", nullptr));
        text1->setText(QCoreApplication::translate("TicketPriceWidget", "\345\207\272\345\217\221\347\253\231", nullptr));
        selectBtn2->setText(QCoreApplication::translate("TicketPriceWidget", "\351\200\211\346\213\251\350\275\246\347\253\231", nullptr));
        text2->setText(QCoreApplication::translate("TicketPriceWidget", "\347\233\256\347\232\204\347\253\231", nullptr));
        selectBtn3->setText(QCoreApplication::translate("TicketPriceWidget", "\351\200\211\346\213\251\350\275\246\347\253\231", nullptr));
        priceCalBtn->setText(QCoreApplication::translate("TicketPriceWidget", "\346\237\245\350\257\242", nullptr));
        priceText->setText(QCoreApplication::translate("TicketPriceWidget", "\345\205\250\347\250\213\347\245\250\344\273\267", nullptr));
        priceUnit->setText(QCoreApplication::translate("TicketPriceWidget", "\345\205\203", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TicketPriceWidget: public Ui_TicketPriceWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TICKETPRICEWIDGET_H
