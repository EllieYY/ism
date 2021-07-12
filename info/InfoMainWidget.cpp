#include "InfoMainWidget.h"
#include "ui_InfoMainWidget.h"
#include "CommonHead.h"
#include "WidgetMng.h"

InfoMainWidget::InfoMainWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::InfoMainWidget)
{
    ui->setupUi(this);
    init();


#if IS_TEST_MODE
    setTestData();
#endif
}

InfoMainWidget::~InfoMainWidget()
{
    delete ui;
}


void InfoMainWidget::registerBtn(QAbstractButton *btn, int id)
{
    this->m_buttonGroup->addButton(btn, id);
}

void InfoMainWidget::onBtn(int btnId)
{
    int widgetIdx[6] =
    {
        MAP_DLG,                   //# 地图导览
        LINE_DLG,                 //# 线路查询
        PRICE_DLG,                //# 票价信息
        TRANSFER_DLG,                   //# 换乘查询
        TIME_DLG,                    //# 时刻表
        PERIPHERY_DLG              //# 地铁周边
    };

    if  (btnId >= 0 && btnId < 6)
    {
        WidgetMng::getThis()->showWidget(widgetIdx[btnId]);
    }
}

void InfoMainWidget::init()
{
    m_buttonGroup = new QButtonGroup(this);
    registerBtn(ui->infoBtn1, 0);
    registerBtn(ui->infoBtn2, 1);
    registerBtn(ui->infoBtn3, 2);
    registerBtn(ui->infoBtn4, 3);
    registerBtn(ui->infoBtn5, 4);
    registerBtn(ui->infoBtn6, 5);

    connect(m_buttonGroup, &QButtonGroup::idClicked, this, &InfoMainWidget::onBtn);

}

void InfoMainWidget::setTestData()
{

}
