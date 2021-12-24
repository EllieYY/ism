#pragma execution_character_set("utf-8")
#include "TDMSummaryTableWgt.h"
#include <QHeaderView>
#include <QDebug>
#include <QTextOption>
#include <QPainter>
#include <QPushButton>
#include <QHBoxLayout>

#include <QScrollBar>
#include <QHeaderView>
#include "LineStationTimetables.h"
#include "StationTime.h"

TDMSummaryTableWgt::TDMSummaryTableWgt(LineStationTimetables* info, QWidget *parent)
    : QTableWidget(parent)
{
    m_info = info;

    initFrozenFrame();
    initFrame();
    initData();

}

TDMSummaryTableWgt::~TDMSummaryTableWgt()
{
    delete m_frozenTableWgt;
}

void TDMSummaryTableWgt::appendRowData(StationTime* timeInfo)
{
//    if (rowCount() >= ROWCOUNTPERPAGE)
//        return;

    //添加到最后一行
    int rowIndex = rowCount();
    setRowCount(rowIndex + 1);

    //设置行高
    setRowHeight(rowIndex, ROWHEIGHT);

    //填充最内容
    setItem(rowIndex, 0, new QTableWidgetItem(timeInfo->stationName()));
    setItem(rowIndex, 1, new QTableWidgetItem(timeInfo->aStartTime()));
    setItem(rowIndex, 2, new QTableWidgetItem(timeInfo->bStartTime()));
    setItem(rowIndex, 3, new QTableWidgetItem(timeInfo->aEndTime()));
    setItem(rowIndex, 4, new QTableWidgetItem(timeInfo->bEndTime()));

    for(int col = 0; col < 5; col++) {
        item(rowIndex, col)->setFont(QFont("Microsoft YaHei",16,500));
        item(rowIndex, col)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}


void TDMSummaryTableWgt::initFrame()
{
    clearContents();
    setColumnCount(5);
    horizontalHeader()->setVisible(true);//表头不可见
    int headerHeight = m_frozenTableWgt->rowHeight(0) +
                       m_frozenTableWgt->rowHeight(1);//设置成被m_frozenTableWgt遮挡住
    horizontalHeader()->setFixedHeight(headerHeight);

    verticalHeader()->setVisible(false);//表头不可见
//    setShowGrid(false);//网格线不可见
    setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    setSelectionMode(QAbstractItemView::NoSelection);//单选
    setSelectionBehavior(QAbstractItemView::SelectRows);//选行`
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    horizontalHeader()->setStretchLastSection(true);//最后一个单元格扩展
    setFocusPolicy(Qt::NoFocus);//解决选中虚框问题
    setFrameShape(QFrame::NoFrame);//去除边框
    setAlternatingRowColors(true);

    setVerticalScrollMode(ScrollPerPixel);
    setHorizontalScrollMode(ScrollPerPixel);

    setItemDelegate(new ItemDelegate(1));//设置绘画代理（主要在代理中画出来header）
}

void TDMSummaryTableWgt::initData()
{
    QList<StationTime*> times = m_info->stationTimes();

    for(StationTime* item:times) {
        appendRowData(item);
    }
}

void TDMSummaryTableWgt::initFrozenFrame()
{
    m_frozenTableWgt = new QTableWidget(this);

    m_frozenTableWgt->horizontalHeader()->setVisible(false);//表头不可见
    m_frozenTableWgt->verticalHeader()->setVisible(false);//表头不可见
    m_frozenTableWgt->setShowGrid(false);//网格线不可见
    m_frozenTableWgt->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
//    m_frozenTableWgt->horizontalHeader()->setStretchLastSection(true);//最后一个单元格扩展
    m_frozenTableWgt->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_frozenTableWgt->setFocusPolicy(Qt::NoFocus);//解决选中虚框问题
    m_frozenTableWgt->setFrameShape(QFrame::HLine);//去除边框 尴尬
    m_frozenTableWgt->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏滚动条
    m_frozenTableWgt->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//
    m_frozenTableWgt->setHorizontalScrollMode(ScrollPerPixel);

    m_frozenTableWgt->setItemDelegate(new ItemDelegate(0));//设置绘画代理（主要在代理中画出来header）

    viewport()->stackUnder(m_frozenTableWgt);//设置窗口层次

    m_frozenTableWgt->setColumnCount(5); //header5列
    m_frozenTableWgt->setRowCount(2);//header2行

    m_frozenTableWgt->setRowHeight(0, 42);//第一行设置高度42px
    m_frozenTableWgt->setRowHeight(1, 42);//第二行设置高度42px

    for (int row = 2; row < m_frozenTableWgt->rowCount(); ++row)//隐藏2行后的行
          m_frozenTableWgt->setRowHidden(row, true);

    //===================设置header内容=================//
    QString dirAStr = QString("往----方向");
    QString dirBStr = QString("往----方向");
    if (m_info != NULL) {
        dirAStr = QString("往%1方向").arg(m_info->dirA());
        dirBStr = QString("往%1方向").arg(m_info->dirB());
    }

    //合并单元格
    m_frozenTableWgt->setSpan(0, 0, 2, 1);//车站
    m_frozenTableWgt->setSpan(0, 1, 1, 2);//首班车
    m_frozenTableWgt->setSpan(0, 3, 1, 2);//末班车

    m_frozenTableWgt->setItem(0, 0, new QTableWidgetItem("车站"));
    m_frozenTableWgt->setItem(0, 1, new QTableWidgetItem("首班车"));
    m_frozenTableWgt->setItem(0, 3, new QTableWidgetItem("末班车"));

    m_frozenTableWgt->setItem(1, 1, new QTableWidgetItem(dirAStr));
    m_frozenTableWgt->setItem(1, 2, new QTableWidgetItem(dirBStr));
    m_frozenTableWgt->setItem(1, 3, new QTableWidgetItem(dirAStr));
    m_frozenTableWgt->setItem(1, 4, new QTableWidgetItem(dirBStr));

    //连接信号槽。用于滚动条联动
    connect(m_frozenTableWgt->verticalScrollBar(), &QAbstractSlider::valueChanged,
            verticalScrollBar(), &QAbstractSlider::setValue);
    connect(verticalScrollBar(), &QAbstractSlider::valueChanged,
            m_frozenTableWgt->verticalScrollBar(), &QAbstractSlider::setValue);

    updateFrozenTableGeometry();//更新位置
    m_frozenTableWgt->show();
}

void TDMSummaryTableWgt::updateFrozenTableGeometry()
{
    m_frozenTableWgt->setGeometry(frameWidth(),
                                 frameWidth(),
                                 viewport()->width(),
                                 horizontalHeader()->height());

}

void TDMSummaryTableWgt::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    updateFrozenTableGeometry();
}

QModelIndex TDMSummaryTableWgt::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);

    if (cursorAction == MoveUp && current.row() > 0
            && visualRect(current).topLeft().y() < m_frozenTableWgt->rowHeight(1) ){
          const int newValue = verticalScrollBar()->value() + visualRect(current).topLeft().y()
                               - m_frozenTableWgt->rowHeight(0) - m_frozenTableWgt->rowHeight(1);
          verticalScrollBar()->setValue(newValue);
    }
    return current;
}

void TDMSummaryTableWgt::scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint)
{
    if (index.row() > 0)
        QTableView::scrollTo(index, hint);

}



ItemDelegate::ItemDelegate(int type, QObject *parent)
    : QStyledItemDelegate(parent),
      m_type(type)
{

}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(0 == m_type) //header tablewidget
    {
        int rowIndex = index.row();//行号
        int colIndex = index.column();//列号
        if (rowIndex == 0 || rowIndex == 1)//前两行作为header
        {
            //背景
            QColor color;
            if (rowIndex == 0 && (colIndex == 0))  //车站
            {
               color.setRgb(47, 181, 163);
            } else if ((rowIndex == 0 && colIndex == 1) || //首班车
                    (rowIndex == 1 && (colIndex == 1 ||
                                       colIndex == 2)))
            {
               color.setRgb(71, 178, 115);
            } else if ((rowIndex == 0 && colIndex == 3) || //末班车
                    (rowIndex == 1 && (colIndex == 3 ||
                                       colIndex == 4)))
            {
               color.setRgb(47, 181, 163);
            }

            //绘制背景
            painter->setPen(color);
            painter->setBrush(QBrush(color));
            painter->drawRect(option.rect);

            //右侧spacer
//            if (rowIndex == 0 && (colIndex == 0 || colIndex == 1)) {
//                int startX = option.rect.right();
//                int startY = option.rect.y() + (option.rect.height() - 40) / 2;
//                int endX = startX;
//                int endY = startY + 40;

//                if (colIndex == 1) {
//                    startY -= 50;
//                    endY -= 50;
//                }
//                QLinearGradient linearGradient(startX, startY, endX, endY);
//                linearGradient.setColorAt(0, QColor(164, 188, 240, 0));
//                linearGradient.setColorAt(0.5, QColor(164, 188, 240, 255));
//                linearGradient.setColorAt(1, QColor(164, 188, 240, 0));
//                painter->setBrush(linearGradient);
//                painter->drawRect(option.rect.right()- 2, startY, 2, 40);

//            } else if (rowIndex == 0 && ) {
//                int startX = option.rect.right();
//                int startY = option.rect.y() + 42 + (option.rect.height() - 40) / 2;
//                int endX = startX;
//                int endY = startY + 82;
//                QLinearGradient linearGradient(startX, startY, endX, endY);
//                linearGradient.setColorAt(0, QColor(164, 188, 240, 0));
//                linearGradient.setColorAt(0.5, QColor(164, 188, 240, 255));
//                linearGradient.setColorAt(1, QColor(164, 188, 240, 0));
//                painter->setBrush(linearGradient);
//                painter->drawRect(option.rect.right()- 2, startY, 2, 40);

//            } else
            if (rowIndex == 1 && (colIndex == 1 ||
                                       colIndex == 3 )) {

                int startX = option.rect.right();
                int startY = option.rect.y() + (option.rect.height() - 28) / 2;
                int endX = startX;
                int endY = startY + 28;
                QLinearGradient linearGradient(startX, startY, endX, endY);
                linearGradient.setColorAt(0, QColor(164, 188, 240, 0));
                linearGradient.setColorAt(0.5, QColor(164, 188, 240, 255));
                linearGradient.setColorAt(1, QColor(164, 188, 240, 0));
                painter->setBrush(linearGradient);
                painter->drawRect(option.rect.right()- 2, startY, 2, 28);
            }

            //字体
            painter->setPen(QColor(255, 255, 255));
            QTextOption op;
            op.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            QFont font;
            font.setFamily("Microsoft YaHei");
            font.setPixelSize(24);
            font.setBold(true);
            painter->setFont(font);

            QRect rect;
            if (rowIndex == 0 && colIndex == 9){//"操作" 左对齐
                rect = QRect(option.rect.x(), option.rect.y(), 100, option.rect.height());
            }
            else {//其他的居中
                rect = option.rect;
            }

            painter->drawText(rect, index.data(Qt::DisplayRole).toString(), op);

        }

    }
    else {  //content table widget
        //文字
        painter->setPen(QColor(25, 25, 25));
        QTextOption op;
        op.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPixelSize(20);
        painter->setFont(font);

        if( option.state & QStyle::State_Selected ) {
            painter->fillRect( option.rect, QBrush(QColor(239,244,255)));
        }

        painter->drawText(option.rect, index.data(Qt::DisplayRole).toString(), op);

        //底边框
        painter->setPen(QColor(248,251,255));
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    }
}

