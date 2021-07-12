#include "MetroPeripheralWidget.h"
#include "ui_MetroPeripheralWidget.h"

MetroPeripheralWidget::MetroPeripheralWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::MetroPeripheralWidget)
{
    ui->setupUi(this);
}

MetroPeripheralWidget::~MetroPeripheralWidget()
{
    delete ui;
}
