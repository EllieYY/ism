#include "MetroInterchangeWidget.h"
#include "ui_MetroInterchangeWidget.h"
#include "CommonHead.h"
#include "DataCenter.h"
#include "LineInterchangeInfo.h"
#include "InterchangeInfo.h"

MetroInterchangeWidget::MetroInterchangeWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::MetroInterchangeWidget)
{
    ui->setupUi(this);

    init();

#if IS_TEST_MODE
//    setTestData();
#endif
}

MetroInterchangeWidget::~MetroInterchangeWidget()
{
    delete ui;
}

bool MetroInterchangeWidget::showData()
{
    // 获取数据
    QList<LineInterchangeInfo*> lineList = DataCenter::getThis()->getLineInterchanges();


    int count = DataCenter::getThis()->getLinesInterchangeCloums(lineList);
    if(count <= 0) return true;
    ui->tableWidget->setRowCount(count);


    // 显示数据
    int row = 0;
    for (LineInterchangeInfo* line : lineList) {

        qDebug() << line;


        int spanSize = line->interchangeList().size();
        if (spanSize > 1) {
            ui->tableWidget->setSpan(row, 0, spanSize, 1);
        }
        QString curLineName = line->getName();
        QTableWidgetItem* item0 = new QTableWidgetItem(curLineName);
        item0->setTextAlignment(Qt::AlignCenter);
        item0->setFont(QFont("Microsoft YaHei",21,500));
        ui->tableWidget->setItem(row, 0, item0);

        QList<InterchangeInfo*> infoList = line->interchangeList();
        for (InterchangeInfo* info : infoList) {
            QTableWidgetItem* item1 = new QTableWidgetItem(info->stationName());
            item1->setTextAlignment(Qt::AlignCenter);
            item1->setFont(QFont("Microsoft YaHei",21,500));
            ui->tableWidget->setItem(row, 1, item1);

            QFrame* frame = new QFrame(this);
            frame->setStyleSheet("background-color: transparent; ");
            frame->setFrameShape(QFrame::NoFrame);
            QHBoxLayout *layout = new QHBoxLayout(frame);
            layout->setMargin(0);
            layout->setSpacing(50);
            QSpacerItem* spaceLR = new QSpacerItem(10, 5, QSizePolicy::Expanding, QSizePolicy::Minimum);
            layout->addSpacerItem(spaceLR);

            QList<LineInfo*> transLines = info->lineList();
            for (LineInfo* transLine:transLines) {
                QPushButton* lineBtn = new QPushButton(transLine->getName(), frame);
                lineBtn->setProperty("type", "lineBtn");
                lineBtn->setStyleSheet("background-color: " + transLine->getColor() + ";");
                layout->addWidget(lineBtn);
            }
//            QPushButton* curBtn = new QPushButton(curLineName, frame);
//            curBtn->setProperty("type", "lineBtn");
//            curBtn->setStyleSheet("background-color: " + line->getColor() + ";");
//            QPushButton* otherBtn = new QPushButton(info->lineName(), frame);
//            otherBtn->setProperty("type", "lineBtn");
//            otherBtn->setStyleSheet("background-color: " + info->lineColor() + ";");


            layout->addSpacerItem(spaceLR);

            ui->tableWidget->setCellWidget(row, 2, frame);

            row++;
        }
    }


    return true;

}

void MetroInterchangeWidget::setStyle()
{
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(60);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->tableWidget->setFrameShape(QFrame::NoFrame);

    ui->tableWidget->verticalHeader()->setVisible(false);                 // 列表头不可见
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 表格不可编辑

    ui->tableWidget->setAlternatingRowColors(true);

}

void MetroInterchangeWidget::init()
{
    setStyle();
}

void MetroInterchangeWidget::setTestData()
{
    QList<LineInterchangeInfo*> lineList;
//    // 测试数据
//    LineInterchangeInfo* line1 = new LineInterchangeInfo("1号线", "", "images/info/line1.png", "#F01C4B");
//    line1->addInterchangeInfo(new InterchangeInfo("地铁大厦", "2号线", "#FFC931"));
//    line1->addInterchangeInfo(new InterchangeInfo("八一馆", "3号线", "#1E5789"));
//    line1->addInterchangeInfo(new InterchangeInfo("八一广场", "2号线", "#FFC931"));

//    LineInterchangeInfo* line2 = new LineInterchangeInfo("2号线", "", "images/info/line2.png", "#FFC931");
//    line2->addInterchangeInfo(new InterchangeInfo("地铁大厦", "1号线", "#F01C4B"));
//    line2->addInterchangeInfo(new InterchangeInfo("青山路口", "3号线", "#1E5789"));
//    line2->addInterchangeInfo(new InterchangeInfo("八一广场", "1号线", "#F01C4B"));

//    LineInterchangeInfo* line3 = new LineInterchangeInfo("3号线", "", "images/info/line3.png", "#1E5789");
//    line3->addInterchangeInfo(new InterchangeInfo("青山路口", "2号线", "#FFC931"));
//    line3->addInterchangeInfo(new InterchangeInfo("八一馆", "1号线", "#F01C4B"));

//    lineList.append(line1);
//    lineList.append(line2);
//    lineList.append(line3);

    DataCenter::getThis()->setLineInterchanges(lineList);

}
