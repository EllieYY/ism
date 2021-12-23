#include "TimeTableWidget.h"
#include "ui_TimeTableWidget.h"
#include "CommonHead.h"
#include "DataCenter.h"
#include "LineTimeTables.h"
#include "ISMTimeTable.h"


TimeTableWidget::TimeTableWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TimeTableWidget)
{
    ui->setupUi(this);
    init();

#if IS_TEST_MODE
//    setTestData();
#endif

}

TimeTableWidget::~TimeTableWidget()
{
    delete ui;
}

//bool TimeTableWidget::showData()
//{
//    // 默认显示4号线
//    showLineTimetables(4);
//    return true;
//}

void TimeTableWidget::showLineTimetables(int line)
{

}

void TimeTableWidget::init()
{
    //TODO： 线路时刻信息获取


    m_initOk = false;
    setStyle();
}


void TimeTableWidget::setStyle()
{
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setColumnWidth(1, 700);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(60);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);

    ui->tableWidget->verticalHeader()->setVisible(false);                 // 列表头不可见
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 表格不可编辑
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection); //设置只能选择一行，不能多行选中


    ui->tableWidget->setFocusPolicy(Qt::NoFocus);    // 虚线框取消

    ui->tableWidget->setAlternatingRowColors(true);
}

void TimeTableWidget::setTestData()
{
    QList<LineTimeTables*> lineList;
    // 测试数据
    LineTimeTables* line1 = new LineTimeTables("1号线", "", "images/info/line1.png", "#F01C4B");
    line1->addTimeTable(new ISMTimeTable("瑶湖西", "双港", "06:00", "22:00"));
    line1->addTimeTable(new ISMTimeTable("双港", "瑶湖西", "06:00", "22:00"));

    LineTimeTables* line2 = new LineTimeTables("2号线", "", "images/info/line2.png", "#FFC931");
    line2->addTimeTable(new ISMTimeTable("南路", "辛家庵", "06:00", "22:00"));
    line2->addTimeTable(new ISMTimeTable("辛家庵", "南路", "06:00", "22:00"));

    LineTimeTables* line3 = new LineTimeTables("3号线", "", "images/info/line3.png", "#1E5789");
    line3->addTimeTable(new ISMTimeTable("银三角北", "京东大道", "06:00", "22:30"));
    line3->addTimeTable(new ISMTimeTable("京东大道", "银三角北", "06:00", "22:30"));

    lineList.append(line1);
    lineList.append(line2);
    lineList.append(line3);

    DataCenter::getThis()->setLineTimeTables(lineList);
}


bool TimeTableWidget::showData()
{
    if (m_initOk) {
        return true;
    }
    m_initOk = true;

    // 获取数据
    QList<LineTimeTables*> lineList = DataCenter::getThis()->getLineTimeTables();
    int count = DataCenter::getThis()->getLinesTimerCloums(lineList);
    if(count <= 0) return true;
    ui->tableWidget->setRowCount(count);


    // 显示数据
    int row = 0;
    for (LineTimeTables* line : lineList) {

        int timeTableSize = line->timeTable().size();
        if (timeTableSize > 1) {
            ui->tableWidget->setSpan(row, 0, timeTableSize, 1);
        }
        QTableWidgetItem* item0 = new QTableWidgetItem(line->getName());
        item0->setTextAlignment(Qt::AlignCenter);
        item0->setFont(QFont("Microsoft YaHei",21,500));
        ui->tableWidget->setItem(row, 0, item0);

        QList<ISMTimeTable*> timeTableList = line->timeTable();
        for (ISMTimeTable* item : timeTableList) {
            QString stationStr = item->startStation() + " -> " + item->endStation();
            QTableWidgetItem* item1 = new QTableWidgetItem(stationStr);
            QTableWidgetItem* item2 = new QTableWidgetItem(item->startTime());
            QTableWidgetItem* item3 = new QTableWidgetItem(item->endTime());

            item1->setTextAlignment(Qt::AlignCenter);
            item2->setTextAlignment(Qt::AlignCenter);
            item3->setTextAlignment(Qt::AlignCenter);

            item1->setFont(QFont("Microsoft YaHei",21,500));
            item2->setFont(QFont("Microsoft YaHei",21,500));
            item3->setFont(QFont("Microsoft YaHei",21,500));

            ui->tableWidget->setItem(row, 1, item1);
            ui->tableWidget->setItem(row, 2, item2);
            ui->tableWidget->setItem(row, 3, item3);
            row++;
        }
    }
    return true;
}
