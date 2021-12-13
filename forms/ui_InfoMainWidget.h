/********************************************************************************
** Form generated from reading UI file 'InfoMainWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFOMAINWIDGET_H
#define UI_INFOMAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InfoMainWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *mainBg;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *bigNameLabel2;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout;
    QPushButton *infoBtn1;
    QPushButton *infoBtn2;
    QPushButton *infoBtn3;
    QPushButton *infoBtn4;
    QPushButton *infoBtn5;
    QPushButton *infoBtn6;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *InfoMainWidget)
    {
        if (InfoMainWidget->objectName().isEmpty())
            InfoMainWidget->setObjectName(QString::fromUtf8("InfoMainWidget"));
        InfoMainWidget->resize(962, 496);
        horizontalLayout = new QHBoxLayout(InfoMainWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        mainBg = new QWidget(InfoMainWidget);
        mainBg->setObjectName(QString::fromUtf8("mainBg"));
        verticalLayout = new QVBoxLayout(mainBg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 99, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        bigNameLabel2 = new QLabel(mainBg);
        bigNameLabel2->setObjectName(QString::fromUtf8("bigNameLabel2"));
        bigNameLabel2->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(bigNameLabel2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 99, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(60);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        infoBtn1 = new QPushButton(mainBg);
        infoBtn1->setObjectName(QString::fromUtf8("infoBtn1"));

        gridLayout->addWidget(infoBtn1, 0, 0, 1, 1);

        infoBtn2 = new QPushButton(mainBg);
        infoBtn2->setObjectName(QString::fromUtf8("infoBtn2"));

        gridLayout->addWidget(infoBtn2, 0, 1, 1, 1);

        infoBtn3 = new QPushButton(mainBg);
        infoBtn3->setObjectName(QString::fromUtf8("infoBtn3"));

        gridLayout->addWidget(infoBtn3, 0, 2, 1, 1);

        infoBtn4 = new QPushButton(mainBg);
        infoBtn4->setObjectName(QString::fromUtf8("infoBtn4"));

        gridLayout->addWidget(infoBtn4, 1, 0, 1, 1);

        infoBtn5 = new QPushButton(mainBg);
        infoBtn5->setObjectName(QString::fromUtf8("infoBtn5"));

        gridLayout->addWidget(infoBtn5, 1, 1, 1, 1);

        infoBtn6 = new QPushButton(mainBg);
        infoBtn6->setObjectName(QString::fromUtf8("infoBtn6"));

        gridLayout->addWidget(infoBtn6, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer_3 = new QSpacerItem(20, 99, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout->addWidget(mainBg);


        retranslateUi(InfoMainWidget);

        QMetaObject::connectSlotsByName(InfoMainWidget);
    } // setupUi

    void retranslateUi(QWidget *InfoMainWidget)
    {
        InfoMainWidget->setWindowTitle(QCoreApplication::translate("InfoMainWidget", "Form", nullptr));
        bigNameLabel2->setText(QCoreApplication::translate("InfoMainWidget", "\344\277\241\346\201\257\346\237\245\350\257\242", nullptr));
        infoBtn1->setText(QCoreApplication::translate("InfoMainWidget", "\345\234\260\345\233\276\345\257\274\350\247\210", nullptr));
        infoBtn2->setText(QCoreApplication::translate("InfoMainWidget", "\347\272\277\350\267\257\344\277\241\346\201\257", nullptr));
        infoBtn3->setText(QCoreApplication::translate("InfoMainWidget", "\347\245\250\344\273\267\344\277\241\346\201\257", nullptr));
        infoBtn4->setText(QCoreApplication::translate("InfoMainWidget", "\346\215\242\344\271\230\346\237\245\350\257\242", nullptr));
        infoBtn5->setText(QCoreApplication::translate("InfoMainWidget", "\346\227\266\345\210\273\350\241\250", nullptr));
        infoBtn6->setText(QCoreApplication::translate("InfoMainWidget", "\345\234\260\351\223\201\345\221\250\350\276\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InfoMainWidget: public Ui_InfoMainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOMAINWIDGET_H
