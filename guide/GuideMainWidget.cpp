#include "GuideMainWidget.h"
#include "ui_GuideMainWidget.h"

GuideMainWidget::GuideMainWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::GuideMainWidget)
{
    ui->setupUi(this);
}

GuideMainWidget::~GuideMainWidget()
{
    delete ui;
}
