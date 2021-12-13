/********************************************************************************
** Form generated from reading UI file 'LineWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINEWIDGET_H
#define UI_LINEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LineWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *lineFrame1;
    QFrame *lineFrame2;
    QVBoxLayout *verticalLayout;
    QLabel *nameLabel;
    QFrame *lineFrame3;

    void setupUi(QWidget *LineWidget)
    {
        if (LineWidget->objectName().isEmpty())
            LineWidget->setObjectName(QString::fromUtf8("LineWidget"));
        LineWidget->resize(986, 529);
        horizontalLayout = new QHBoxLayout(LineWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        lineFrame1 = new QFrame(LineWidget);
        lineFrame1->setObjectName(QString::fromUtf8("lineFrame1"));
        lineFrame1->setMinimumSize(QSize(280, 0));
        lineFrame1->setFrameShape(QFrame::StyledPanel);
        lineFrame1->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(lineFrame1);

        lineFrame2 = new QFrame(LineWidget);
        lineFrame2->setObjectName(QString::fromUtf8("lineFrame2"));
        lineFrame2->setFrameShape(QFrame::StyledPanel);
        lineFrame2->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(lineFrame2);
        verticalLayout->setSpacing(9);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 31, 0, 0);
        nameLabel = new QLabel(lineFrame2);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(nameLabel);

        lineFrame3 = new QFrame(lineFrame2);
        lineFrame3->setObjectName(QString::fromUtf8("lineFrame3"));
        lineFrame3->setFrameShape(QFrame::StyledPanel);
        lineFrame3->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(lineFrame3);

        verticalLayout->setStretch(1, 1);

        horizontalLayout->addWidget(lineFrame2);

        horizontalLayout->setStretch(1, 1);

        retranslateUi(LineWidget);

        QMetaObject::connectSlotsByName(LineWidget);
    } // setupUi

    void retranslateUi(QWidget *LineWidget)
    {
        LineWidget->setWindowTitle(QCoreApplication::translate("LineWidget", "Form", nullptr));
        nameLabel->setText(QCoreApplication::translate("LineWidget", "\345\215\227\346\230\214\350\275\250\351\201\223\344\272\244\351\200\232\347\272\277\347\275\221\347\244\272\346\204\217\345\233\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LineWidget: public Ui_LineWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINEWIDGET_H
