#include "MapWidget.h"
#include "ui_MapWidget.h"

MapWidget::MapWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::MapWidget)
{
    ui->setupUi(this);
}

MapWidget::~MapWidget()
{
    delete ui;
}
