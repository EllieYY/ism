/********************************************************************************
** Form generated from reading UI file 'GuideMainWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUIDEMAINWIDGET_H
#define UI_GUIDEMAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GuideMainWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *guideBg;
    QHBoxLayout *horizontalLayout_10;
    QFrame *guideFrame;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_7;
    QGridLayout *gridLayout;
    QLabel *guideCh;
    QSpacerItem *verticalSpacer_14;
    QLabel *guideEn;
    QSpacerItem *verticalSpacer_15;
    QLabel *nameLabel;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_8;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *guide1;
    QPushButton *guide2;
    QPushButton *guide3;
    QPushButton *guide4;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer;
    QFrame *frame1;
    QHBoxLayout *horizontalLayout_4;
    QFrame *bgFrame1;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_11;
    QSpacerItem *horizontalSpacer_10;
    QFrame *iconFrame1;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *verticalSpacer_9;
    QVBoxLayout *verticalLayout;
    QLabel *guideType1;
    QLabel *guideTypeDes1;
    QSpacerItem *verticalSpacer_10;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout_2;
    QLabel *numlabel1;
    QLabel *gText1;
    QLabel *numlabel2;
    QLabel *gText2;
    QLabel *numlabel3;
    QLabel *gText3;
    QLabel *numlabel4;
    QLabel *gText4;
    QLabel *numlabel5;
    QLabel *gText5;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QFrame *frame2;
    QHBoxLayout *horizontalLayout_5;
    QFrame *bgFrame2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_12;
    QSpacerItem *horizontalSpacer_12;
    QFrame *iconFrame2;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *verticalSpacer_11;
    QVBoxLayout *verticalLayout_5;
    QLabel *guideType2;
    QLabel *guideTypeDes2;
    QSpacerItem *verticalSpacer_12;
    QVBoxLayout *verticalLayout_6;
    QSpacerItem *verticalSpacer_3;
    QGridLayout *gridLayout_3;
    QLabel *numlabel6;
    QLabel *gText6;
    QLabel *numlabel7;
    QLabel *gText7;
    QLabel *numlabel8;
    QLabel *gText8;
    QLabel *numlabel9;
    QLabel *gText9;
    QLabel *numlabel10;
    QLabel *gText10;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_13;
    QSpacerItem *verticalSpacer_6;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_3;
    QLabel *tipLabel;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *verticalSpacer_7;

    void setupUi(QWidget *GuideMainWidget)
    {
        if (GuideMainWidget->objectName().isEmpty())
            GuideMainWidget->setObjectName(QString::fromUtf8("GuideMainWidget"));
        GuideMainWidget->resize(1230, 903);
        horizontalLayout = new QHBoxLayout(GuideMainWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        guideBg = new QWidget(GuideMainWidget);
        guideBg->setObjectName(QString::fromUtf8("guideBg"));
        horizontalLayout_10 = new QHBoxLayout(guideBg);
        horizontalLayout_10->setSpacing(0);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(30, 30, 30, 30);
        guideFrame = new QFrame(guideBg);
        guideFrame->setObjectName(QString::fromUtf8("guideFrame"));
        guideFrame->setFrameShape(QFrame::StyledPanel);
        guideFrame->setFrameShadow(QFrame::Raised);
        verticalLayout_7 = new QVBoxLayout(guideFrame);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 87, 0, 78);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_7);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        guideCh = new QLabel(guideFrame);
        guideCh->setObjectName(QString::fromUtf8("guideCh"));

        gridLayout->addWidget(guideCh, 3, 1, 1, 1);

        verticalSpacer_14 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_14, 1, 1, 1, 1);

        guideEn = new QLabel(guideFrame);
        guideEn->setObjectName(QString::fromUtf8("guideEn"));
        guideEn->setMinimumSize(QSize(56, 23));
        guideEn->setMaximumSize(QSize(56, 23));
        guideEn->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(guideEn, 2, 1, 1, 1);

        verticalSpacer_15 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_15, 4, 1, 1, 1);

        nameLabel = new QLabel(guideFrame);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(nameLabel, 1, 0, 4, 1);


        horizontalLayout_7->addLayout(gridLayout);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);


        verticalLayout_7->addLayout(horizontalLayout_7);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_7->addItem(verticalSpacer_8);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(40);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        guide1 = new QPushButton(guideFrame);
        guide1->setObjectName(QString::fromUtf8("guide1"));
        guide1->setMinimumSize(QSize(150, 50));
        guide1->setMaximumSize(QSize(150, 50));

        horizontalLayout_2->addWidget(guide1);

        guide2 = new QPushButton(guideFrame);
        guide2->setObjectName(QString::fromUtf8("guide2"));
        guide2->setMinimumSize(QSize(150, 50));
        guide2->setMaximumSize(QSize(150, 50));

        horizontalLayout_2->addWidget(guide2);

        guide3 = new QPushButton(guideFrame);
        guide3->setObjectName(QString::fromUtf8("guide3"));
        guide3->setMinimumSize(QSize(150, 50));
        guide3->setMaximumSize(QSize(150, 50));

        horizontalLayout_2->addWidget(guide3);

        guide4 = new QPushButton(guideFrame);
        guide4->setObjectName(QString::fromUtf8("guide4"));
        guide4->setMinimumSize(QSize(150, 50));
        guide4->setMaximumSize(QSize(150, 50));

        horizontalLayout_2->addWidget(guide4);


        horizontalLayout_8->addLayout(horizontalLayout_2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);


        verticalLayout_7->addLayout(horizontalLayout_8);

        verticalSpacer_5 = new QSpacerItem(20, 68, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_7->addItem(verticalSpacer_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        frame1 = new QFrame(guideFrame);
        frame1->setObjectName(QString::fromUtf8("frame1"));
        frame1->setMinimumSize(QSize(0, 366));
        frame1->setFrameShape(QFrame::StyledPanel);
        frame1->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame1);
        horizontalLayout_4->setSpacing(56);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 90, 0);
        bgFrame1 = new QFrame(frame1);
        bgFrame1->setObjectName(QString::fromUtf8("bgFrame1"));
        bgFrame1->setFrameShape(QFrame::StyledPanel);
        bgFrame1->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(bgFrame1);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(35, 80, 52, -1);
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_10);

        iconFrame1 = new QFrame(bgFrame1);
        iconFrame1->setObjectName(QString::fromUtf8("iconFrame1"));
        iconFrame1->setMinimumSize(QSize(80, 80));
        iconFrame1->setMaximumSize(QSize(80, 80));
        iconFrame1->setFrameShape(QFrame::StyledPanel);
        iconFrame1->setFrameShadow(QFrame::Raised);

        horizontalLayout_11->addWidget(iconFrame1);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_11);


        verticalLayout_2->addLayout(horizontalLayout_11);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_9);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        guideType1 = new QLabel(bgFrame1);
        guideType1->setObjectName(QString::fromUtf8("guideType1"));
        guideType1->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(guideType1);

        guideTypeDes1 = new QLabel(bgFrame1);
        guideTypeDes1->setObjectName(QString::fromUtf8("guideTypeDes1"));
        guideTypeDes1->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(guideTypeDes1);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_10);


        horizontalLayout_4->addWidget(bgFrame1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(32);
        gridLayout_2->setVerticalSpacing(24);
        numlabel1 = new QLabel(frame1);
        numlabel1->setObjectName(QString::fromUtf8("numlabel1"));
        numlabel1->setMinimumSize(QSize(36, 36));
        numlabel1->setMaximumSize(QSize(36, 36));
        numlabel1->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(numlabel1, 0, 0, 1, 1);

        gText1 = new QLabel(frame1);
        gText1->setObjectName(QString::fromUtf8("gText1"));

        gridLayout_2->addWidget(gText1, 0, 1, 1, 1);

        numlabel2 = new QLabel(frame1);
        numlabel2->setObjectName(QString::fromUtf8("numlabel2"));
        numlabel2->setMinimumSize(QSize(36, 36));
        numlabel2->setMaximumSize(QSize(36, 36));
        numlabel2->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(numlabel2, 1, 0, 1, 1);

        gText2 = new QLabel(frame1);
        gText2->setObjectName(QString::fromUtf8("gText2"));

        gridLayout_2->addWidget(gText2, 1, 1, 1, 1);

        numlabel3 = new QLabel(frame1);
        numlabel3->setObjectName(QString::fromUtf8("numlabel3"));
        numlabel3->setMinimumSize(QSize(36, 36));
        numlabel3->setMaximumSize(QSize(36, 36));
        numlabel3->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(numlabel3, 2, 0, 1, 1);

        gText3 = new QLabel(frame1);
        gText3->setObjectName(QString::fromUtf8("gText3"));

        gridLayout_2->addWidget(gText3, 2, 1, 1, 1);

        numlabel4 = new QLabel(frame1);
        numlabel4->setObjectName(QString::fromUtf8("numlabel4"));
        numlabel4->setMinimumSize(QSize(36, 36));
        numlabel4->setMaximumSize(QSize(36, 36));
        numlabel4->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(numlabel4, 3, 0, 1, 1);

        gText4 = new QLabel(frame1);
        gText4->setObjectName(QString::fromUtf8("gText4"));

        gridLayout_2->addWidget(gText4, 3, 1, 1, 1);

        numlabel5 = new QLabel(frame1);
        numlabel5->setObjectName(QString::fromUtf8("numlabel5"));
        numlabel5->setMinimumSize(QSize(36, 36));
        numlabel5->setMaximumSize(QSize(36, 36));
        numlabel5->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(numlabel5, 4, 0, 1, 1);

        gText5 = new QLabel(frame1);
        gText5->setObjectName(QString::fromUtf8("gText5"));

        gridLayout_2->addWidget(gText5, 4, 1, 1, 1);


        verticalLayout_3->addLayout(gridLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout_4->addLayout(verticalLayout_3);


        horizontalLayout_6->addWidget(frame1);

        horizontalSpacer_3 = new QSpacerItem(100, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        frame2 = new QFrame(guideFrame);
        frame2->setObjectName(QString::fromUtf8("frame2"));
        frame2->setMinimumSize(QSize(0, 366));
        frame2->setFrameShape(QFrame::StyledPanel);
        frame2->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame2);
        horizontalLayout_5->setSpacing(56);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 90, 0);
        bgFrame2 = new QFrame(frame2);
        bgFrame2->setObjectName(QString::fromUtf8("bgFrame2"));
        bgFrame2->setFrameShape(QFrame::StyledPanel);
        bgFrame2->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(bgFrame2);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(35, 80, 52, -1);
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_12);

        iconFrame2 = new QFrame(bgFrame2);
        iconFrame2->setObjectName(QString::fromUtf8("iconFrame2"));
        iconFrame2->setMinimumSize(QSize(80, 80));
        iconFrame2->setMaximumSize(QSize(80, 80));
        iconFrame2->setFrameShape(QFrame::StyledPanel);
        iconFrame2->setFrameShadow(QFrame::Raised);

        horizontalLayout_12->addWidget(iconFrame2);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_13);


        verticalLayout_4->addLayout(horizontalLayout_12);

        verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer_11);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        guideType2 = new QLabel(bgFrame2);
        guideType2->setObjectName(QString::fromUtf8("guideType2"));
        guideType2->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(guideType2);

        guideTypeDes2 = new QLabel(bgFrame2);
        guideTypeDes2->setObjectName(QString::fromUtf8("guideTypeDes2"));
        guideTypeDes2->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(guideTypeDes2);


        verticalLayout_4->addLayout(verticalLayout_5);

        verticalSpacer_12 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_12);


        horizontalLayout_5->addWidget(bgFrame2);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalSpacer_3 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_3);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setHorizontalSpacing(32);
        gridLayout_3->setVerticalSpacing(24);
        numlabel6 = new QLabel(frame2);
        numlabel6->setObjectName(QString::fromUtf8("numlabel6"));
        numlabel6->setMinimumSize(QSize(36, 36));
        numlabel6->setMaximumSize(QSize(36, 36));
        numlabel6->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(numlabel6, 0, 0, 1, 1);

        gText6 = new QLabel(frame2);
        gText6->setObjectName(QString::fromUtf8("gText6"));

        gridLayout_3->addWidget(gText6, 0, 1, 1, 1);

        numlabel7 = new QLabel(frame2);
        numlabel7->setObjectName(QString::fromUtf8("numlabel7"));
        numlabel7->setMinimumSize(QSize(36, 36));
        numlabel7->setMaximumSize(QSize(36, 36));
        numlabel7->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(numlabel7, 1, 0, 1, 1);

        gText7 = new QLabel(frame2);
        gText7->setObjectName(QString::fromUtf8("gText7"));

        gridLayout_3->addWidget(gText7, 1, 1, 1, 1);

        numlabel8 = new QLabel(frame2);
        numlabel8->setObjectName(QString::fromUtf8("numlabel8"));
        numlabel8->setMinimumSize(QSize(36, 36));
        numlabel8->setMaximumSize(QSize(36, 36));
        numlabel8->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(numlabel8, 2, 0, 1, 1);

        gText8 = new QLabel(frame2);
        gText8->setObjectName(QString::fromUtf8("gText8"));

        gridLayout_3->addWidget(gText8, 2, 1, 1, 1);

        numlabel9 = new QLabel(frame2);
        numlabel9->setObjectName(QString::fromUtf8("numlabel9"));
        numlabel9->setMinimumSize(QSize(36, 36));
        numlabel9->setMaximumSize(QSize(36, 36));
        numlabel9->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(numlabel9, 3, 0, 1, 1);

        gText9 = new QLabel(frame2);
        gText9->setObjectName(QString::fromUtf8("gText9"));

        gridLayout_3->addWidget(gText9, 3, 1, 1, 1);

        numlabel10 = new QLabel(frame2);
        numlabel10->setObjectName(QString::fromUtf8("numlabel10"));
        numlabel10->setMinimumSize(QSize(36, 36));
        numlabel10->setMaximumSize(QSize(36, 36));
        numlabel10->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(numlabel10, 4, 0, 1, 1);

        gText10 = new QLabel(frame2);
        gText10->setObjectName(QString::fromUtf8("gText10"));

        gridLayout_3->addWidget(gText10, 4, 1, 1, 1);


        verticalLayout_6->addLayout(gridLayout_3);

        verticalSpacer_4 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_4);


        horizontalLayout_5->addLayout(verticalLayout_6);


        horizontalLayout_6->addWidget(frame2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);


        verticalLayout_7->addLayout(horizontalLayout_6);

        verticalSpacer_13 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_7->addItem(verticalSpacer_13);

        verticalSpacer_6 = new QSpacerItem(20, 7, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_6);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_8);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(14);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        tipLabel = new QLabel(guideFrame);
        tipLabel->setObjectName(QString::fromUtf8("tipLabel"));
        tipLabel->setMinimumSize(QSize(25, 25));
        tipLabel->setMaximumSize(QSize(25, 25));

        horizontalLayout_3->addWidget(tipLabel);

        label_7 = new QLabel(guideFrame);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_3->addWidget(label_7);


        horizontalLayout_9->addLayout(horizontalLayout_3);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_9);


        verticalLayout_7->addLayout(horizontalLayout_9);

        verticalSpacer_7 = new QSpacerItem(20, 6, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_7);


        horizontalLayout_10->addWidget(guideFrame);


        horizontalLayout->addWidget(guideBg);


        retranslateUi(GuideMainWidget);

        QMetaObject::connectSlotsByName(GuideMainWidget);
    } // setupUi

    void retranslateUi(QWidget *GuideMainWidget)
    {
        GuideMainWidget->setWindowTitle(QCoreApplication::translate("GuideMainWidget", "Form", nullptr));
        guideCh->setText(QCoreApplication::translate("GuideMainWidget", "\346\214\207\345\274\225", nullptr));
        guideEn->setText(QCoreApplication::translate("GuideMainWidget", "GUIDE", nullptr));
        nameLabel->setText(QCoreApplication::translate("GuideMainWidget", "\346\231\272\350\203\275\345\256\242\346\234\215\346\223\215\344\275\234", nullptr));
        guide1->setText(QCoreApplication::translate("GuideMainWidget", "\346\231\272\346\205\247\347\224\237\346\264\273", nullptr));
        guide2->setText(QCoreApplication::translate("GuideMainWidget", "\347\234\201\346\227\266\347\234\201\344\272\213", nullptr));
        guide3->setText(QCoreApplication::translate("GuideMainWidget", "\346\226\271\344\276\277\345\277\253\346\215\267", nullptr));
        guide4->setText(QCoreApplication::translate("GuideMainWidget", "\350\207\252\344\270\273\346\223\215\344\275\234", nullptr));
        guideType1->setText(QCoreApplication::translate("GuideMainWidget", "\347\245\250\345\215\241", nullptr));
        guideTypeDes1->setText(QCoreApplication::translate("GuideMainWidget", "\346\227\240\346\263\225\350\277\233\345\207\272\347\253\231", nullptr));
        numlabel1->setText(QCoreApplication::translate("GuideMainWidget", "1", nullptr));
        gText1->setText(QCoreApplication::translate("GuideMainWidget", "\351\200\211\346\213\251\347\245\250\345\215\241\345\244\204\347\220\206", nullptr));
        numlabel2->setText(QCoreApplication::translate("GuideMainWidget", "2", nullptr));
        gText2->setText(QCoreApplication::translate("GuideMainWidget", "\351\200\211\346\213\251\347\245\250\345\215\241\350\241\245\347\231\273", nullptr));
        numlabel3->setText(QCoreApplication::translate("GuideMainWidget", "3", nullptr));
        gText3->setText(QCoreApplication::translate("GuideMainWidget", "\351\200\211\346\213\251\350\277\233/\345\207\272\347\253\231", nullptr));
        numlabel4->setText(QCoreApplication::translate("GuideMainWidget", "4", nullptr));
        gText4->setText(QCoreApplication::translate("GuideMainWidget", "\351\200\211\346\213\251\346\224\257\344\273\230\346\226\271\345\274\217", nullptr));
        numlabel5->setText(QCoreApplication::translate("GuideMainWidget", "5", nullptr));
        gText5->setText(QCoreApplication::translate("GuideMainWidget", "\347\202\271\345\207\273\342\200\234\347\241\256\350\256\244/\345\210\267\346\226\260\342\200\235", nullptr));
        guideType2->setText(QCoreApplication::translate("GuideMainWidget", "\344\272\214\347\273\264\347\240\201", nullptr));
        guideTypeDes2->setText(QCoreApplication::translate("GuideMainWidget", "\346\227\240\346\263\225\350\277\233\345\207\272\347\253\231", nullptr));
        numlabel6->setText(QCoreApplication::translate("GuideMainWidget", "1", nullptr));
        gText6->setText(QCoreApplication::translate("GuideMainWidget", "\351\200\211\346\213\251\344\272\214\347\273\264\347\240\201", nullptr));
        numlabel7->setText(QCoreApplication::translate("GuideMainWidget", "2", nullptr));
        gText7->setText(QCoreApplication::translate("GuideMainWidget", "\351\200\211\346\213\251\344\272\214\347\273\264\347\240\201\350\241\245\347\231\273", nullptr));
        numlabel8->setText(QCoreApplication::translate("GuideMainWidget", "3", nullptr));
        gText8->setText(QCoreApplication::translate("GuideMainWidget", "\346\211\253\346\217\217\344\271\230\350\275\246\347\240\201", nullptr));
        numlabel9->setText(QCoreApplication::translate("GuideMainWidget", "4", nullptr));
        gText9->setText(QCoreApplication::translate("GuideMainWidget", "\351\200\211\346\213\251\350\275\246\347\253\231", nullptr));
        numlabel10->setText(QCoreApplication::translate("GuideMainWidget", "5", nullptr));
        gText10->setText(QCoreApplication::translate("GuideMainWidget", "\347\202\271\345\207\273\342\200\234\347\241\256\350\256\244\342\200\235\350\241\245\347\231\273", nullptr));
        tipLabel->setText(QString());
        label_7->setText(QCoreApplication::translate("GuideMainWidget", "\346\270\251\351\246\250\346\217\220\347\244\272\357\274\232\345\246\202\351\234\200\345\270\256\345\212\251\357\274\214\350\257\267\346\214\211\345\216\213\342\200\234\346\213\233\346\217\264\342\200\235\346\214\211\351\222\256\345\221\274\345\217\253\345\267\245\344\275\234\344\272\272\345\221\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GuideMainWidget: public Ui_GuideMainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIDEMAINWIDGET_H
