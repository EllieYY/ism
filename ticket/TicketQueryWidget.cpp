#include "TicketQueryWidget.h"
#include "ui_TicketQueryWidget.h"
#include "CommonHead.h"
#include "DataCenter.h"
#include "TicketBasicInfo.h"
#include "TransactionInfo.h"
#include "CardReadWidget.h"

TicketQueryWidget::TicketQueryWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TicketQueryWidget)
{
    ui->setupUi(this);

    init();

#if IS_TEST_MODE
    setTestData();
#endif
}

TicketQueryWidget::~TicketQueryWidget()
{
    delete ui;
}

void TicketQueryWidget::secEvent()
{
   showData();
}

bool TicketQueryWidget::showData()
{
    // 票卡基本信息 -- 更新
    if (m_dataUpdateNum[TICKET_BASIC])
    {
        m_dataUpdateNum[TICKET_BASIC] = false;

        showTicketBasicInfo();
    }

    // 票卡交易信息 -- 更新
    if (m_dataUpdateNum[TICKET_TRANS])
    {
        m_dataUpdateNum[TICKET_TRANS] = false;
        showTicketTransInfo();
    }
    return true;
}

void TicketQueryWidget::showTicketBasicInfo()
{
    // 车票基本信息
    TicketBasicInfo* tInfo = DataCenter::getThis()->getTicketBasicInfo();
    QList<QTableWidgetItem*> itemList = DataCenter::getThis()->getTicketItems(tInfo);

    int index = 0;
    for (QTableWidgetItem* item:itemList) {
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(QFont("Microsoft YaHei",18,500));
        item->setForeground(QColor("#09262A"));

        ui->tableWidget->setItem(0, index++, item);
    }

}

void TicketQueryWidget::showTicketTransInfo()
{
    // 乘车明细
    QList<TransactionInfo*> transInfoList = DataCenter::getThis()->getTransInfoList();
    int count = transInfoList.size();
    if(count <= 0)
        return;

    ui->tableWidget_2->setRowCount(count);

    int row = 0;
    for (TransactionInfo* info:transInfoList) {

        QList<QTableWidgetItem*> itemList = DataCenter::getThis()->getTranscationItems(info);
        int subIndex = 0;
        for (QTableWidgetItem* item:itemList) {
            item->setTextAlignment(Qt::AlignCenter);
            item->setFont(QFont("Microsoft YaHei",16,500));
            item->setForeground(QColor("#7B8081"));
            ui->tableWidget_2->setItem(row, subIndex++, item);
        }
        row++;
    }

}


void TicketQueryWidget::init()
{
    setStyle();
}


void TicketQueryWidget::setStyle()
{
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);    // 虚线框取消
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(70);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    ui->tableWidget->verticalHeader()->setVisible(false);                 // 列表头不可见
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 表格不可编辑
//    ui->tableWidget->setAlternatingRowColors(true);


    ui->tableWidget_2->setFocusPolicy(Qt::NoFocus);    // 虚线框取消
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->verticalHeader()->setVisible(false);
    ui->tableWidget_2->verticalHeader()->setDefaultSectionSize(70);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->setFrameShape(QFrame::NoFrame);
    ui->tableWidget_2->verticalHeader()->setVisible(false);                 // 列表头不可见
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 表格不可编辑
    ui->tableWidget_2->setAlternatingRowColors(true);
    ui->tableWidget_2->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{background: #C1EAD8;font-size: 28px;color: #09262A;}");
}

void TicketQueryWidget::setTestData()
{
    // 票卡基本信息
//    TicketBasicInfo* info = new TicketBasicInfo(
//                "一卡通", "20001379",
//                QDate(2019, 10, 1), QDate(2022, 10, 1),
//                0, 1, 18.0);
//    DataCenter::getThis()->setTicketBasicInfo(info);

    // 乘车明细信息
//    QList<TransactionInfo*> transInfoList;
//    transInfoList.append(new TransactionInfo(QDateTime::currentDateTime(), 1, 3.6, 2, 46.4, 10));
//    transInfoList.append(new TransactionInfo(QDateTime::currentDateTime(), 1, 3.6, 2, 46.4, 10));
//    transInfoList.append(new TransactionInfo(QDateTime::currentDateTime(), 1, 3.6, 2, 46.4, 10));
//    transInfoList.append(new TransactionInfo(QDateTime::currentDateTime(), 1, 3.6, 2, 46.4, 10));
//    transInfoList.append(new TransactionInfo(QDateTime::currentDateTime(), 1, 3.6, 2, 46.4, 10));
//    transInfoList.append(new TransactionInfo(QDateTime::currentDateTime(), 1, 3.6, 2, 46.4, 10));
//    transInfoList.append(new TransactionInfo(QDateTime::currentDateTime(), 1, 3.6, 2, 46.4, 10));
//    transInfoList.append(new TransactionInfo(QDateTime::currentDateTime(), 1, 3.6, 2, 46.4, 10));
//    transInfoList.append(new TransactionInfo(QDateTime::currentDateTime(), 1, 3.6, 2, 46.4, 10));
//    DataCenter::getThis()->setTransInfoList(transInfoList);
}
