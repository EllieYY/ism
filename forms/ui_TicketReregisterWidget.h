/********************************************************************************
** Form generated from reading UI file 'TicketReregisterWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TICKETREREGISTERWIDGET_H
#define UI_TICKETREREGISTERWIDGET_H

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

class Ui_TicketReregisterWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *infoBg;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_3;
    QLabel *nameLabel;
    QSpacerItem *verticalSpacer_2;
    QFrame *centerFrame;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *text1;
    QLineEdit *lineEdit1;
    QSpacerItem *horizontalSpacer;
    QLabel *text5;
    QLineEdit *lineEdit5;
    QPushButton *selectBtn3;
    QLabel *text2;
    QLineEdit *lineEdit2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *text6;
    QLineEdit *lineEdit6;
    QSpacerItem *horizontalSpacer_4;
    QLabel *text3;
    QLineEdit *lineEdit3;
    QPushButton *selectBtn2;
    QLabel *text7;
    QLineEdit *lineEdit7;
    QSpacerItem *horizontalSpacer_5;
    QLabel *text4;
    QLineEdit *lineEdit4;
    QSpacerItem *horizontalSpacer_3;
    QLabel *text8;
    QLineEdit *lineEdit8;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *updateBtn;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *okBtn;

    void setupUi(QWidget *TicketReregisterWidget)
    {
        if (TicketReregisterWidget->objectName().isEmpty())
            TicketReregisterWidget->setObjectName(QString::fromUtf8("TicketReregisterWidget"));
        TicketReregisterWidget->resize(1053, 614);
        horizontalLayout = new QHBoxLayout(TicketReregisterWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        infoBg = new QWidget(TicketReregisterWidget);
        infoBg->setObjectName(QString::fromUtf8("infoBg"));
        verticalLayout_2 = new QVBoxLayout(infoBg);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(50, 0, 50, 50);
        verticalSpacer_3 = new QSpacerItem(20, 35, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_3);

        nameLabel = new QLabel(infoBg);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(nameLabel);

        verticalSpacer_2 = new QSpacerItem(20, 33, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_2);

        centerFrame = new QFrame(infoBg);
        centerFrame->setObjectName(QString::fromUtf8("centerFrame"));
        centerFrame->setFrameShape(QFrame::StyledPanel);
        centerFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(centerFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(70, 50, 70, 50);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(30);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        text1 = new QLabel(centerFrame);
        text1->setObjectName(QString::fromUtf8("text1"));
        text1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text1, 0, 0, 1, 1);

        lineEdit1 = new QLineEdit(centerFrame);
        lineEdit1->setObjectName(QString::fromUtf8("lineEdit1"));

        gridLayout->addWidget(lineEdit1, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        text5 = new QLabel(centerFrame);
        text5->setObjectName(QString::fromUtf8("text5"));
        text5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text5, 0, 3, 1, 1);

        lineEdit5 = new QLineEdit(centerFrame);
        lineEdit5->setObjectName(QString::fromUtf8("lineEdit5"));

        gridLayout->addWidget(lineEdit5, 0, 4, 1, 1);

        selectBtn3 = new QPushButton(centerFrame);
        selectBtn3->setObjectName(QString::fromUtf8("selectBtn3"));

        gridLayout->addWidget(selectBtn3, 0, 5, 1, 1);

        text2 = new QLabel(centerFrame);
        text2->setObjectName(QString::fromUtf8("text2"));
        text2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text2, 1, 0, 1, 1);

        lineEdit2 = new QLineEdit(centerFrame);
        lineEdit2->setObjectName(QString::fromUtf8("lineEdit2"));

        gridLayout->addWidget(lineEdit2, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        text6 = new QLabel(centerFrame);
        text6->setObjectName(QString::fromUtf8("text6"));
        text6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text6, 1, 3, 1, 1);

        lineEdit6 = new QLineEdit(centerFrame);
        lineEdit6->setObjectName(QString::fromUtf8("lineEdit6"));

        gridLayout->addWidget(lineEdit6, 1, 4, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 1, 5, 1, 1);

        text3 = new QLabel(centerFrame);
        text3->setObjectName(QString::fromUtf8("text3"));
        text3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text3, 2, 0, 1, 1);

        lineEdit3 = new QLineEdit(centerFrame);
        lineEdit3->setObjectName(QString::fromUtf8("lineEdit3"));

        gridLayout->addWidget(lineEdit3, 2, 1, 1, 1);

        selectBtn2 = new QPushButton(centerFrame);
        selectBtn2->setObjectName(QString::fromUtf8("selectBtn2"));

        gridLayout->addWidget(selectBtn2, 2, 2, 1, 1);

        text7 = new QLabel(centerFrame);
        text7->setObjectName(QString::fromUtf8("text7"));
        text7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text7, 2, 3, 1, 1);

        lineEdit7 = new QLineEdit(centerFrame);
        lineEdit7->setObjectName(QString::fromUtf8("lineEdit7"));

        gridLayout->addWidget(lineEdit7, 2, 4, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 2, 5, 1, 1);

        text4 = new QLabel(centerFrame);
        text4->setObjectName(QString::fromUtf8("text4"));
        text4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text4, 3, 0, 1, 1);

        lineEdit4 = new QLineEdit(centerFrame);
        lineEdit4->setObjectName(QString::fromUtf8("lineEdit4"));

        gridLayout->addWidget(lineEdit4, 3, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 3, 2, 1, 1);

        text8 = new QLabel(centerFrame);
        text8->setObjectName(QString::fromUtf8("text8"));
        text8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text8, 3, 3, 1, 1);

        lineEdit8 = new QLineEdit(centerFrame);
        lineEdit8->setObjectName(QString::fromUtf8("lineEdit8"));

        gridLayout->addWidget(lineEdit8, 3, 4, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 3, 5, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 150, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        updateBtn = new QPushButton(centerFrame);
        updateBtn->setObjectName(QString::fromUtf8("updateBtn"));

        horizontalLayout_3->addWidget(updateBtn);

        horizontalSpacer_8 = new QSpacerItem(60, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_8);

        okBtn = new QPushButton(centerFrame);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));

        horizontalLayout_3->addWidget(okBtn);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(centerFrame);


        horizontalLayout->addWidget(infoBg);


        retranslateUi(TicketReregisterWidget);

        QMetaObject::connectSlotsByName(TicketReregisterWidget);
    } // setupUi

    void retranslateUi(QWidget *TicketReregisterWidget)
    {
        TicketReregisterWidget->setWindowTitle(QCoreApplication::translate("TicketReregisterWidget", "Form", nullptr));
        nameLabel->setText(QCoreApplication::translate("TicketReregisterWidget", "\350\257\267\347\241\256\350\256\244\347\245\250\345\215\241\344\277\241\346\201\257\345\271\266\351\200\211\346\213\251\350\275\246\347\253\231", nullptr));
        text1->setText(QCoreApplication::translate("TicketReregisterWidget", "\350\275\246\347\245\250\347\247\215\347\261\273", nullptr));
        text5->setText(QCoreApplication::translate("TicketReregisterWidget", "\345\207\272\347\253\231\350\275\246\347\253\231", nullptr));
        selectBtn3->setText(QCoreApplication::translate("TicketReregisterWidget", "\351\200\211\346\213\251\350\275\246\347\253\231", nullptr));
        text2->setText(QCoreApplication::translate("TicketReregisterWidget", "\350\275\246\347\245\250\347\212\266\346\200\201", nullptr));
        text6->setText(QCoreApplication::translate("TicketReregisterWidget", "\346\234\211\346\225\210\346\234\237\351\231\220", nullptr));
        text3->setText(QCoreApplication::translate("TicketReregisterWidget", "\350\277\233\347\253\231\350\275\246\347\253\231", nullptr));
        selectBtn2->setText(QCoreApplication::translate("TicketReregisterWidget", "\351\200\211\346\213\251\350\275\246\347\253\231", nullptr));
        text7->setText(QCoreApplication::translate("TicketReregisterWidget", "\350\241\245\347\245\250\351\207\221\351\242\235\357\274\210\345\205\203\357\274\211", nullptr));
        text4->setText(QCoreApplication::translate("TicketReregisterWidget", "\350\277\233\347\253\231\346\227\266\351\227\264", nullptr));
        text8->setText(QCoreApplication::translate("TicketReregisterWidget", "\350\241\245\347\245\250\345\216\237\345\233\240", nullptr));
        updateBtn->setText(QCoreApplication::translate("TicketReregisterWidget", "\345\210\267\346\226\260", nullptr));
        okBtn->setText(QCoreApplication::translate("TicketReregisterWidget", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TicketReregisterWidget: public Ui_TicketReregisterWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TICKETREREGISTERWIDGET_H
