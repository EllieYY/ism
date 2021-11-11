#include "LineWidget.h"
#include "ui_LineWidget.h"
#include "CommonHead.h"
#include "LineInfo.h"
#include "DataCenter.h"

#include <QVBoxLayout>

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
    int lineNum = m_lineInfoLst.size();
    for (int i = 0; i < lineNum; i++){
        QPushButton* btn = new QPushButton(this);
        btn->setText(m_lineInfoLst[i]->getName());
        btn->setProperty("pname", "line");
        btn->setStyleSheet("background-color: " + m_lineInfoLst[i]->getColor() + ";");
        m_layout->addWidget(btn);
        m_btnGroup->addButton(btn, i);

        if (i == 0) {
            btn->setChecked(true);
            onLineChange(i);
        }
    }

    m_layout->setContentsMargins(42, 60, 42, 1);
    m_layout->setSpacing(60);
    m_layout->addSpacerItem(new QSpacerItem(5, 100, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(m_btnGroup, &QButtonGroup::idClicked,
            this, &LineWidget::onLineChange);
    m_btnGroup->setExclusive(true);

    ui->lineFrame1->setLayout(m_layout);
}

void LineWidget::onLineChange(int id)
{
    if (id < 0 || id >= m_lineInfoLst.size()) {
        return;
    }

    QString styleStr = "border-image: url(images/info/"+ m_lineInfoLst[id]->getPicPath() + ");";
    ui->lineFrame3->setStyleSheet(styleStr);
}

bool LineWidget::showData()
{
    onReadLines();
}
