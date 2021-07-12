#include "LineWidget.h"
#include "ui_LineWidget.h"
#include "CommonHead.h"
#include "LineInfo.h"

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
    //TODO:获取配置文件
    QList<LineInfo*> lineInfoLst;
    LineInfo* item = new LineInfo("1号线", "line1.png", "#F01C4B", this);
    lineInfoLst.append(item);
    item = new LineInfo("2号线", "line2.png", "#FFC931", this);
    lineInfoLst.append(item);
    item = new LineInfo("3号线", "line3.png", "#1E5789", this);
    lineInfoLst.append(item);
    item = new LineInfo("4号线（在建）", "line4.png", "#3BA954", this);
    lineInfoLst.append(item);

    QVBoxLayout *layout = new QVBoxLayout();

    m_lineInfoLst.append(lineInfoLst);
    m_btnGroup = new QButtonGroup(this);
    int lineNum = lineInfoLst.size();
    for (int i = 0; i < lineNum; i++){
        QPushButton* btn = new QPushButton(this);
        btn->setText(lineInfoLst[i]->getName());
        btn->setProperty("pname", "line");
        btn->setStyleSheet("background-color: " + lineInfoLst[i]->getColor() + ";");
        layout->addWidget(btn);
        m_btnGroup->addButton(btn, i);

        if (i == 0) {
            btn->setChecked(true);
            onLineChange(i);
        }
    }

    layout->setContentsMargins(42, 60, 42, 1);
    layout->setSpacing(60);
    layout->addSpacerItem(new QSpacerItem(5, 100, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(m_btnGroup, &QButtonGroup::idClicked,
            this, &LineWidget::onLineChange);
    m_btnGroup->setExclusive(true);


    ui->lineFrame1->setLayout(layout);



}

void LineWidget::onLineChange(int id)
{
    if (id < 0 || id >= m_lineInfoLst.size()) {
        return;
    }

    QString styleStr = "border-image: url(images/info/"+ m_lineInfoLst[id]->getPicPath() + ");";
    ui->lineFrame3->setStyleSheet(styleStr);
}
