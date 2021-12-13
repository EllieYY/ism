/********************************************************************************
** Form generated from reading UI file 'QrCodeMainWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QRCODEMAINWIDGET_H
#define UI_QRCODEMAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QrCodeMainWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *mainBg;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *bigNameLabel2;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QToolButton *codeQueryBtn;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *codeReregisterBtn;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *QrCodeMainWidget)
    {
        if (QrCodeMainWidget->objectName().isEmpty())
            QrCodeMainWidget->setObjectName(QString::fromUtf8("QrCodeMainWidget"));
        QrCodeMainWidget->resize(976, 481);
        verticalLayout_2 = new QVBoxLayout(QrCodeMainWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        mainBg = new QWidget(QrCodeMainWidget);
        mainBg->setObjectName(QString::fromUtf8("mainBg"));
        mainBg->setAutoFillBackground(true);
        verticalLayout = new QVBoxLayout(mainBg);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 81, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        bigNameLabel2 = new QLabel(mainBg);
        bigNameLabel2->setObjectName(QString::fromUtf8("bigNameLabel2"));
        bigNameLabel2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(bigNameLabel2);

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
        codeQueryBtn = new QToolButton(mainBg);
        codeQueryBtn->setObjectName(QString::fromUtf8("codeQueryBtn"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(codeQueryBtn->sizePolicy().hasHeightForWidth());
        codeQueryBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(codeQueryBtn);

        horizontalSpacer_3 = new QSpacerItem(240, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        codeReregisterBtn = new QToolButton(mainBg);
        codeReregisterBtn->setObjectName(QString::fromUtf8("codeReregisterBtn"));
        sizePolicy.setHeightForWidth(codeReregisterBtn->sizePolicy().hasHeightForWidth());
        codeReregisterBtn->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(codeReregisterBtn);

        horizontalLayout->setStretch(0, 31);
        horizontalLayout->setStretch(1, 5);
        horizontalLayout->setStretch(2, 31);

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

        verticalLayout_2->addWidget(mainBg);


        retranslateUi(QrCodeMainWidget);

        QMetaObject::connectSlotsByName(QrCodeMainWidget);
    } // setupUi

    void retranslateUi(QWidget *QrCodeMainWidget)
    {
        QrCodeMainWidget->setWindowTitle(QCoreApplication::translate("QrCodeMainWidget", "Form", nullptr));
        bigNameLabel2->setText(QCoreApplication::translate("QrCodeMainWidget", "\344\272\214\347\273\264\347\240\201", nullptr));
        codeQueryBtn->setText(QCoreApplication::translate("QrCodeMainWidget", "\344\272\214\347\273\264\347\240\201\346\237\245\350\257\242", nullptr));
        codeReregisterBtn->setText(QCoreApplication::translate("QrCodeMainWidget", "\344\272\214\347\273\264\347\240\201\350\241\245\347\274\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QrCodeMainWidget: public Ui_QrCodeMainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QRCODEMAINWIDGET_H
