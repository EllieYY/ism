/********************************************************************************
** Form generated from reading UI file 'InquiryMainWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INQUIRYMAINWIDGET_H
#define UI_INQUIRYMAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InquiryMainWidget
{
public:
    QVBoxLayout *verticalLayout_3;
    QWidget *infoBg5;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_3;
    QLabel *nameLabel;
    QSpacerItem *verticalSpacer_2;
    QFrame *centerFrame;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QListWidget *msgListWidget;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer;
    QPushButton *voice;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *send;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *hotTitleIcon;
    QLabel *hotTitle;
    QSpacerItem *horizontalSpacer_3;
    QListWidget *hotListWidget;

    void setupUi(QWidget *InquiryMainWidget)
    {
        if (InquiryMainWidget->objectName().isEmpty())
            InquiryMainWidget->setObjectName(QString::fromUtf8("InquiryMainWidget"));
        InquiryMainWidget->resize(1027, 564);
        verticalLayout_3 = new QVBoxLayout(InquiryMainWidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        infoBg5 = new QWidget(InquiryMainWidget);
        infoBg5->setObjectName(QString::fromUtf8("infoBg5"));
        verticalLayout_2 = new QVBoxLayout(infoBg5);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(50, 0, 50, 50);
        verticalSpacer_3 = new QSpacerItem(20, 35, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_3);

        nameLabel = new QLabel(infoBg5);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(nameLabel);

        verticalSpacer_2 = new QSpacerItem(20, 33, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_2);

        centerFrame = new QFrame(infoBg5);
        centerFrame->setObjectName(QString::fromUtf8("centerFrame"));
        centerFrame->setFrameShape(QFrame::StyledPanel);
        centerFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(centerFrame);
        horizontalLayout_3->setSpacing(30);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(30, 30, 30, 30);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(30);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        msgListWidget = new QListWidget(centerFrame);
        msgListWidget->setObjectName(QString::fromUtf8("msgListWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(msgListWidget->sizePolicy().hasHeightForWidth());
        msgListWidget->setSizePolicy(sizePolicy);
        msgListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout->addWidget(msgListWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit = new QLineEdit(centerFrame);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 60));
        lineEdit->setMaximumSize(QSize(16777215, 60));

        horizontalLayout->addWidget(lineEdit);

        horizontalSpacer = new QSpacerItem(32, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        voice = new QPushButton(centerFrame);
        voice->setObjectName(QString::fromUtf8("voice"));
        voice->setMinimumSize(QSize(60, 60));
        voice->setMaximumSize(QSize(60, 60));

        horizontalLayout->addWidget(voice);

        horizontalSpacer_2 = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        send = new QPushButton(centerFrame);
        send->setObjectName(QString::fromUtf8("send"));
        send->setMinimumSize(QSize(150, 60));
        send->setMaximumSize(QSize(150, 60));

        horizontalLayout->addWidget(send);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_3->addLayout(verticalLayout);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(24);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        hotTitleIcon = new QLabel(centerFrame);
        hotTitleIcon->setObjectName(QString::fromUtf8("hotTitleIcon"));
        hotTitleIcon->setMinimumSize(QSize(37, 37));
        hotTitleIcon->setMaximumSize(QSize(37, 37));

        horizontalLayout_2->addWidget(hotTitleIcon);

        hotTitle = new QLabel(centerFrame);
        hotTitle->setObjectName(QString::fromUtf8("hotTitle"));

        horizontalLayout_2->addWidget(hotTitle);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout_4->addLayout(horizontalLayout_2);

        hotListWidget = new QListWidget(centerFrame);
        hotListWidget->setObjectName(QString::fromUtf8("hotListWidget"));
        hotListWidget->setFrameShape(QFrame::NoFrame);

        verticalLayout_4->addWidget(hotListWidget);


        horizontalLayout_3->addLayout(verticalLayout_4);

        horizontalLayout_3->setStretch(0, 3);
        horizontalLayout_3->setStretch(1, 1);

        verticalLayout_2->addWidget(centerFrame);

        verticalLayout_2->setStretch(3, 1);

        verticalLayout_3->addWidget(infoBg5);


        retranslateUi(InquiryMainWidget);

        QMetaObject::connectSlotsByName(InquiryMainWidget);
    } // setupUi

    void retranslateUi(QWidget *InquiryMainWidget)
    {
        InquiryMainWidget->setWindowTitle(QCoreApplication::translate("InquiryMainWidget", "Form", nullptr));
        nameLabel->setText(QCoreApplication::translate("InquiryMainWidget", "\346\231\272\350\203\275\351\227\256\350\257\242", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("InquiryMainWidget", "\350\257\267\347\224\250\344\270\200\345\217\245\350\257\235\347\256\200\350\246\201\343\200\201\345\207\206\347\241\256\346\217\217\350\277\260\346\202\250\347\232\204\351\227\256\351\242\230", nullptr));
        voice->setText(QString());
        send->setText(QCoreApplication::translate("InquiryMainWidget", "\345\217\221\351\200\201", nullptr));
        hotTitleIcon->setText(QString());
        hotTitle->setText(QCoreApplication::translate("InquiryMainWidget", "\347\203\255\351\227\250\351\227\256\351\242\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InquiryMainWidget: public Ui_InquiryMainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INQUIRYMAINWIDGET_H
