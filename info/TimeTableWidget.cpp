#include "TimeTableWidget.h"
#include "ui_TimeTableWidget.h"
#include "CommonHead.h"
#include "DataCenter.h"
#include "LineTimeTables.h"
#include "ISMTimeTable.h"
#include "SettingCenter.h"
#include "RbTableHeaderView.h"
#include "LineStationTimetables.h"
#include "StationTime.h"
#include "TDMSummaryTableWgt.h"


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

bool TimeTableWidget::showData()
{
    // 默认显示4号线
    onBtn(4);
    return true;
}

void TimeTableWidget::showLineTimetables(QTableView* view, LineStationTimetables* lineInfo)
{
//    ui->tableView->clear();
    if (lineInfo == NULL) {
        return;
    }

    QString dirAStr = QString("往%1方向").arg(lineInfo->dirA());
    QString dirBStr = QString("往%1方向").arg(lineInfo->dirB());
    RbTableHeaderView* hHead = new RbTableHeaderView(Qt::Horizontal,2,5);
    hHead->setColumnWidth(0,288);
    hHead->setColumnWidth(1,288);
    hHead->setColumnWidth(2,288);
    hHead->setColumnWidth(3,288);
    hHead->setColumnWidth(4,288);
    hHead->setRowHeight(0,60);
    hHead->setRowHeight(1,60);

    QAbstractItemModel* hModel = hHead->model();
    QStandardItemModel* dataModel = new QStandardItemModel;

    hHead->setSpan(0,0,2,0);
    hHead->setSpan(0,1,0,2);
    hHead->setSpan(0,3,0,2);
    hModel->setData(hModel->index(0,0),QString("车站"),Qt::DisplayRole);
    hModel->setData(hModel->index(0,1),QString("首班车"),Qt::DisplayRole);
    hModel->setData(hModel->index(0,3),QString("末班车"),Qt::DisplayRole);
    hModel->setData(hModel->index(1,1),QString(dirAStr),Qt::DisplayRole);
    hModel->setData(hModel->index(1,2),QString(dirBStr),Qt::DisplayRole);
    hModel->setData(hModel->index(1,3),QString(dirAStr),Qt::DisplayRole);
    hModel->setData(hModel->index(1,4),QString(dirBStr),Qt::DisplayRole);

//    headerModel->setData(headerModel->index(row,col),0x009944,Qt::BackgroundRole);
//    headerModel->setData(headerModel->index(row,col),0xFFFFFF,Qt::ForegroundRole);

//    hHead->setCellBackgroundColor(hModel->index(0,0), 0x009944);
//    hHead->setCellBackgroundColor(hModel->index(0,1), 0x009944);
//    hHead->setCellBackgroundColor(hModel->index(0,3), 0x009944);
//    hHead->setCellBackgroundColor(hModel->index(1,1), 0x009944);
//    hHead->setCellBackgroundColor(hModel->index(1,2), 0x009944);
//    hHead->setCellBackgroundColor(hModel->index(1,3), 0x009944);
//    hHead->setCellBackgroundColor(hModel->index(1,4), 0x009944);

    hHead->setSectionResizeMode(0, QHeaderView::Fixed);
    hHead->setSectionResizeMode(2, QHeaderView::Stretch);


    QList<StationTime*> stationList = lineInfo->stationTimes();
    for (StationTime* item : stationList) {
        QList<QStandardItem*> items;
        items.append(new QStandardItem(item->stationName()));
        items.append(new QStandardItem(item->aStartTime()));
        items.append(new QStandardItem(item->bStartTime()));
        items.append(new QStandardItem(item->aEndTime()));
        items.append(new QStandardItem(item->bEndTime()));

        dataModel->appendRow(items);
    }

    // 此处调用顺序，setHorizontalHeader务必在后
    view->setModel(dataModel);
    view->setHorizontalHeader(hHead);
}

void TimeTableWidget::onBtn(int id)
{
    if (m_preLineView.view != NULL) {
        m_preLineView.view->hide();
    }
    if (m_preLineView.btn != NULL) {
        m_preLineView.btn->setStyleSheet(QString("background-color: #cfdbd5;"));
    }

    QTableWidget* view = m_lineViewMap.value(id).view;
    QPushButton* btn = m_lineViewMap.value(id).btn;
    btn->setStyleSheet(QString("background-color: rgba(0,153,68,70%);"));

    m_preLineView.view = view;
    m_preLineView.btn = btn;
    m_preLineView.btnId = id;
    view->show();
}


void TimeTableWidget::init()
{
    m_preTableView = NULL;
    m_preTableWidget = NULL;
    m_preLineView.btn = NULL;
    m_preLineView.view = NULL;

    //线路时刻信息获取
    this->m_lineTimes.insert(SettingCenter::getThis()->getLineStationTimetables());
    QList<int> keys = m_lineTimes.keys();
    std::sort(keys.begin(), keys.end());

    QVBoxLayout* viewLayout = new QVBoxLayout;
    QHBoxLayout* btnLayout = new QHBoxLayout;

    QButtonGroup* buttonGroup = new QButtonGroup(this);
    for (int key:keys) {
        QString btnName = QString("%1号线").arg(key);
        QPushButton* btn = new QPushButton(btnName, this);
        btn->setProperty("btnType", "lineTime");

        LineStationTimetables* info = m_lineTimes.value(key);
        TDMSummaryTableWgt* widget = new TDMSummaryTableWgt(info, this);

        buttonGroup->addButton(btn, key);

        LINE_VIEW viewStruct;
        viewStruct.btn = btn;
        viewStruct.view = widget;
        viewStruct.btnId = key;
        m_lineViewMap.insert(key, viewStruct);

        viewLayout->addWidget(widget);
        btnLayout->addWidget(btn);
        widget->hide();
    }

    buttonGroup->setExclusive(true);
    connect(buttonGroup, &QButtonGroup::idClicked, this, &TimeTableWidget::onBtn);

    viewLayout->setMargin(0);
    viewLayout->setSpacing(0);
    ui->viewFrame->setLayout(viewLayout);
    ui->btnFrame->setLayout(btnLayout);

    m_initOk = false;
    setStyle();
}


void TimeTableWidget::setViewStyle(QTableView *view)
{
    view->verticalHeader()->setVisible(false);                 // 列表头不可见
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 表格不可编辑
    view->setSelectionMode(QAbstractItemView::NoSelection); //设置只能选择一行，不能多行选中
    view->setFocusPolicy(Qt::NoFocus);    // 虚线框取消
    view->setAlternatingRowColors(true);
}

void TimeTableWidget::setStyle()
{
    ui->tableWidget->hide();
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setColumnWidth(1, 700);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(60);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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


//bool TimeTableWidget::showData()
//{
//    if (m_initOk) {
//        return true;
//    }
//    m_initOk = true;

//    // 获取数据
//    QList<LineTimeTables*> lineList = DataCenter::getThis()->getLineTimeTables();
//    int count = DataCenter::getThis()->getLinesTimerCloums(lineList);
//    if(count <= 0) return true;
//    ui->tableWidget->setRowCount(count);


//    // 显示数据
//    int row = 0;
//    for (LineTimeTables* line : lineList) {

//        int timeTableSize = line->timeTable().size();
//        if (timeTableSize > 1) {
//            ui->tableWidget->setSpan(row, 0, timeTableSize, 1);
//        }
//        QTableWidgetItem* item0 = new QTableWidgetItem(line->getName());
//        item0->setTextAlignment(Qt::AlignCenter);
//        item0->setFont(QFont("Microsoft YaHei",21,500));
//        ui->tableWidget->setItem(row, 0, item0);

//        QList<ISMTimeTable*> timeTableList = line->timeTable();
//        for (ISMTimeTable* item : timeTableList) {
//            QString stationStr = item->startStation() + " -> " + item->endStation();
//            QTableWidgetItem* item1 = new QTableWidgetItem(stationStr);
//            QTableWidgetItem* item2 = new QTableWidgetItem(item->startTime());
//            QTableWidgetItem* item3 = new QTableWidgetItem(item->endTime());

//            item1->setTextAlignment(Qt::AlignCenter);
//            item2->setTextAlignment(Qt::AlignCenter);
//            item3->setTextAlignment(Qt::AlignCenter);

//            item1->setFont(QFont("Microsoft YaHei",21,500));
//            item2->setFont(QFont("Microsoft YaHei",21,500));
//            item3->setFont(QFont("Microsoft YaHei",21,500));

//            ui->tableWidget->setItem(row, 1, item1);
//            ui->tableWidget->setItem(row, 2, item2);
//            ui->tableWidget->setItem(row, 3, item3);
//            row++;
//        }
//    }
//    return true;
//}
