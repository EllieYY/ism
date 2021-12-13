/********************************************************************************
** Form generated from reading UI file 'StationSelectWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATIONSELECTWIDGET_H
#define UI_STATIONSELECTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StationSelectWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *selectBg;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QFrame *selectFrame;
    QVBoxLayout *verticalLayout;
    QWidget *selectHead;
    QHBoxLayout *horizontalLayout_2;
    QLabel *selectText;
    QSpacerItem *horizontalSpacer;
    QPushButton *closeBtn;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lineText;
    QLabel *stationText;
    QFrame *frame;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *StationSelectWidget)
    {
        if (StationSelectWidget->objectName().isEmpty())
            StationSelectWidget->setObjectName(QString::fromUtf8("StationSelectWidget"));
        StationSelectWidget->resize(902, 589);
        horizontalLayout = new QHBoxLayout(StationSelectWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        selectBg = new QWidget(StationSelectWidget);
        selectBg->setObjectName(QString::fromUtf8("selectBg"));
        verticalLayout_2 = new QVBoxLayout(selectBg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(20, 120, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(261, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        selectFrame = new QFrame(selectBg);
        selectFrame->setObjectName(QString::fromUtf8("selectFrame"));
        selectFrame->setFrameShape(QFrame::StyledPanel);
        selectFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(selectFrame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        selectHead = new QWidget(selectFrame);
        selectHead->setObjectName(QString::fromUtf8("selectHead"));
        selectHead->setMinimumSize(QSize(0, 70));
        selectHead->setMaximumSize(QSize(16777215, 70));
        horizontalLayout_2 = new QHBoxLayout(selectHead);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        selectText = new QLabel(selectHead);
        selectText->setObjectName(QString::fromUtf8("selectText"));
        selectText->setMargin(20);

        horizontalLayout_2->addWidget(selectText);

        horizontalSpacer = new QSpacerItem(174, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        closeBtn = new QPushButton(selectHead);
        closeBtn->setObjectName(QString::fromUtf8("closeBtn"));
        closeBtn->setMinimumSize(QSize(70, 70));
        closeBtn->setMaximumSize(QSize(16777215, 70));

        horizontalLayout_2->addWidget(closeBtn);


        verticalLayout->addWidget(selectHead);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lineText = new QLabel(selectFrame);
        lineText->setObjectName(QString::fromUtf8("lineText"));
        lineText->setMinimumSize(QSize(236, 80));
        lineText->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(lineText);

        stationText = new QLabel(selectFrame);
        stationText->setObjectName(QString::fromUtf8("stationText"));
        stationText->setMinimumSize(QSize(0, 80));
        stationText->setMargin(20);

        horizontalLayout_3->addWidget(stationText);

        horizontalLayout_3->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout_3);

        frame = new QFrame(selectFrame);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(frame);

        verticalLayout->setStretch(2, 1);

        horizontalLayout_4->addWidget(selectFrame);

        horizontalSpacer_3 = new QSpacerItem(260, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer_2 = new QSpacerItem(20, 108, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        verticalLayout_2->setStretch(1, 1);

        horizontalLayout->addWidget(selectBg);


        retranslateUi(StationSelectWidget);

        QMetaObject::connectSlotsByName(StationSelectWidget);
    } // setupUi

    void retranslateUi(QWidget *StationSelectWidget)
    {
        StationSelectWidget->setWindowTitle(QCoreApplication::translate("StationSelectWidget", "Form", nullptr));
        selectText->setText(QCoreApplication::translate("StationSelectWidget", "\351\200\211\346\213\251\347\253\231\347\202\271 ", nullptr));
        closeBtn->setText(QString());
        lineText->setText(QCoreApplication::translate("StationSelectWidget", "\347\272\277\350\267\257", nullptr));
        stationText->setText(QCoreApplication::translate("StationSelectWidget", "\347\253\231\347\202\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StationSelectWidget: public Ui_StationSelectWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATIONSELECTWIDGET_H
