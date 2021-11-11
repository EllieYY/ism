#include "RefundWidget.h"
#include "ui_RefundWidget.h"
#include "CommonHead.h"
#include "DataCenter.h"
#include "TicketBasicInfo.h"

RefundWidget::RefundWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::RefundWidget)
{
    ui->setupUi(this);

    init();

#if IS_TEST_MODE
    setTestData();
#endif

}

RefundWidget::~RefundWidget()
{
    delete ui;
}


bool RefundWidget::showData()
{
    // TODO:触发车票信息获取
    TicketBasicInfo* info = DataCenter::getThis()->getTicketBasicInfo();
    QList<QTableWidgetItem*> itemList = DataCenter::getThis()->getTicketItems(info);

    int index = 0;
    for (QTableWidgetItem* item:itemList) {
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont("Microsoft YaHei",18,500));
        item->setForeground(QColor("#09262A"));

        ui->tableWidget->setItem(0, index++, item);
    }

    return true;
}



void RefundWidget::init()
{
    setStyle();
}


void RefundWidget::setStyle()
{
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(70);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);

    ui->tableWidget->verticalHeader()->setVisible(false);                 // 列表头不可见
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 表格不可编辑

    ui->tableWidget->setAlternatingRowColors(true);
}

void RefundWidget::setTestData()
{
//    TicketBasicInfo* info = new TicketBasicInfo("一卡通", "20001379", QDate(2019, 10, 1), QDate(2022, 10, 1), 0, 1, 18.0);

//    DataCenter::getThis()->setTicketBasicInfo(info);
}
