#include "LineWidget.h"
#include "ui_LineWidget.h"
#include "CommonHead.h"
#include "LineInfo.h"
#include "DataCenter.h"

#include <QVBoxLayout>

#define LINE_ALL_ID  60

LineWidget::LineWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::LineWidget)
{
    ui->setupUi(this);

    init();
}

LineWidget::~LineWidget()
{
    delete ui;
}

void LineWidget::init()
{
    m_layout = NULL;
    connect(DataCenter::getThis(), &DataCenter::lineReceived, this, &LineWidget::onReadLines);
}

void LineWidget::onReadLines()
{
    m_lineInfoLst.clear();
    m_lineInfoLst = DataCenter::getThis()->getLineList();

//    if (NULL != m_layout) {
//        delete m_layout;
//    }
    m_layout = new QVBoxLayout();

//    delete m_btnGroup;
    m_btnGroup = new QButtonGroup(this);

    // 线路总图
    QPushButton* btnAll = new QPushButton(this);
    btnAll->setText("运营线路图");
    btnAll->setProperty("pname", "line");
    btnAll->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #26B6B5, stop:1 #62AF3E);" );
    m_layout->addWidget(btnAll);
    m_btnGroup->addButton(btnAll, LINE_ALL_ID);


    int lineNum = m_lineInfoLst.size();
    for (int i = 0; i < lineNum; i++){
        QPushButton* btn = new QPushButton(this);
        btn->setText(m_lineInfoLst[i]->getName());
        btn->setProperty("pname", "line");
        btn->setStyleSheet("background-color: " + m_lineInfoLst[i]->getColor() + ";");
        m_layout->addWidget(btn);
        m_btnGroup->addButton(btn, i);
    }

    m_layout->setContentsMargins(42, 60, 42, 1);
    m_layout->setSpacing(60);
    m_layout->addSpacerItem(new QSpacerItem(5, 100, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(m_btnGroup, &QButtonGroup::idClicked,
            this, &LineWidget::onLineChange);
    m_btnGroup->setExclusive(true);

    ui->lineFrame1->setLayout(m_layout);

    btnAll->setChecked(true);
    onLineChange(LINE_ALL_ID);
}

void LineWidget::onLineChange(int id)
{
    QString styleStr = "border-image: url(images/info/lineAll.jpg);";
    if (id == LINE_ALL_ID) {
        ui->nameLabel->setText("南昌轨道交通运营线路图");
        ui->lineFrame3->setStyleSheet(styleStr);
        return;
    }

    if (id < 0 || id >= m_lineInfoLst.size()) {
        return;
    }

    ui->nameLabel->setText(m_lineInfoLst[id]->getName() + "线路图");
    styleStr = "border-image: url(images/info/"+ m_lineInfoLst[id]->getPicPath() + ");";
    ui->lineFrame3->setStyleSheet(styleStr);
}

bool LineWidget::showData()
{
    onReadLines();
}
