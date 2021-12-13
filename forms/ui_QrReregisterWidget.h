/********************************************************************************
** Form generated from reading UI file 'QrReregisterWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QRREREGISTERWIDGET_H
#define UI_QRREREGISTERWIDGET_H

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

class Ui_QrReregisterWidget
{
public:
    QVBoxLayout *verticalLayout_3;
    QWidget *infoBg;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_3;
    QLabel *nameLabel;
    QSpacerItem *verticalSpacer_2;
    QFrame *centerFrame;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *text1;
    QLabel *text2;
    QLineEdit *lineEdit2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *text8;
    QLineEdit *lineEdit8;
    QSpacerItem *horizontalSpacer_9;
    QLabel *text3;
    QLineEdit *lineEdit3;
    QPushButton *selectBtn2;
    QSpacerItem *horizontalSpacer_4;
    QLabel *text5;
    QLineEdit *lineEdit5;
    QPushButton *selectBtn3;
    QLabel *text4;
    QLineEdit *lineEdit4;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *lineEdit1;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *updateBtn;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *okBtn;

    void setupUi(QWidget *QrReregisterWidget)
    {
        if (QrReregisterWidget->objectName().isEmpty())
            QrReregisterWidget->setObjectName(QString::fromUtf8("QrReregisterWidget"));
        QrReregisterWidget->resize(893, 485);
        verticalLayout_3 = new QVBoxLayout(QrReregisterWidget);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        infoBg = new QWidget(QrReregisterWidget);
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
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(70, 50, 70, 50);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(30);
        text1 = new QLabel(centerFrame);
        text1->setObjectName(QString::fromUtf8("text1"));
        text1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text1, 0, 0, 1, 1);

        text2 = new QLabel(centerFrame);
        text2->setObjectName(QString::fromUtf8("text2"));
        text2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text2, 1, 0, 1, 1);

        lineEdit2 = new QLineEdit(centerFrame);
        lineEdit2->setObjectName(QString::fromUtf8("lineEdit2"));

        gridLayout->addWidget(lineEdit2, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        text8 = new QLabel(centerFrame);
        text8->setObjectName(QString::fromUtf8("text8"));
        text8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text8, 1, 4, 1, 1);

        lineEdit8 = new QLineEdit(centerFrame);
        lineEdit8->setObjectName(QString::fromUtf8("lineEdit8"));

        gridLayout->addWidget(lineEdit8, 1, 5, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_9, 1, 6, 1, 1);

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

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 2, 3, 1, 1);

        text5 = new QLabel(centerFrame);
        text5->setObjectName(QString::fromUtf8("text5"));
        text5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text5, 2, 4, 1, 1);

        lineEdit5 = new QLineEdit(centerFrame);
        lineEdit5->setObjectName(QString::fromUtf8("lineEdit5"));

        gridLayout->addWidget(lineEdit5, 2, 5, 1, 1);

        selectBtn3 = new QPushButton(centerFrame);
        selectBtn3->setObjectName(QString::fromUtf8("selectBtn3"));

        gridLayout->addWidget(selectBtn3, 2, 6, 1, 1);

        text4 = new QLabel(centerFrame);
        text4->setObjectName(QString::fromUtf8("text4"));
        text4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(text4, 3, 0, 1, 1);

        lineEdit4 = new QLineEdit(centerFrame);
        lineEdit4->setObjectName(QString::fromUtf8("lineEdit4"));

        gridLayout->addWidget(lineEdit4, 3, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(448, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 3, 2, 1, 5);

        lineEdit1 = new QLineEdit(centerFrame);
        lineEdit1->setObjectName(QString::fromUtf8("lineEdit1"));

        gridLayout->addWidget(lineEdit1, 0, 1, 1, 6);


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


        verticalLayout_3->addWidget(infoBg);


        retranslateUi(QrReregisterWidget);

        QMetaObject::connectSlotsByName(QrReregisterWidget);
    } // setupUi

    void retranslateUi(QWidget *QrReregisterWidget)
    {
        QrReregisterWidget->setWindowTitle(QCoreApplication::translate("QrReregisterWidget", "Form", nullptr));
        nameLabel->setText(QCoreApplication::translate("QrReregisterWidget", "\350\257\267\347\241\256\350\256\244\344\272\214\347\273\264\347\240\201\344\277\241\346\201\257\345\271\266\351\200\211\346\213\251\350\275\246\347\253\231", nullptr));
        text1->setText(QCoreApplication::translate("QrReregisterWidget", "\346\217\220\347\244\272\344\277\241\346\201\257", nullptr));
        text2->setText(QCoreApplication::translate("QrReregisterWidget", "\346\270\240\351\201\223", nullptr));
        text8->setText(QCoreApplication::translate("QrReregisterWidget", "\346\230\257\345\220\246\345\217\257\344\273\245\350\241\245\347\231\273", nullptr));
        text3->setText(QCoreApplication::translate("QrReregisterWidget", "\350\277\233\347\253\231\350\275\246\347\253\231", nullptr));
        selectBtn2->setText(QCoreApplication::translate("QrReregisterWidget", "\351\200\211\346\213\251\350\275\246\347\253\231", nullptr));
        text5->setText(QCoreApplication::translate("QrReregisterWidget", "\345\207\272\347\253\231\350\275\246\347\253\231", nullptr));
        selectBtn3->setText(QCoreApplication::translate("QrReregisterWidget", "\351\200\211\346\213\251\350\275\246\347\253\231", nullptr));
        text4->setText(QCoreApplication::translate("QrReregisterWidget", "\350\277\233\347\253\231\346\227\266\351\227\264", nullptr));
        updateBtn->setText(QCoreApplication::translate("QrReregisterWidget", "\345\210\267\346\226\260", nullptr));
        okBtn->setText(QCoreApplication::translate("QrReregisterWidget", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QrReregisterWidget: public Ui_QrReregisterWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QRREREGISTERWIDGET_H
