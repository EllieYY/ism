#include "PaymentWidget.h"
#include "ui_PaymentWidget.h"

PaymentWidget::PaymentWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::PaymentWidget)
{
    ui->setupUi(this);
    init();

}

PaymentWidget::~PaymentWidget()
{
    delete ui;
}

void PaymentWidget::init()
{
    ui->aliPayBtn->setCheckable(true);
    ui->wePayBtn->setCheckable(true);
    ui->cashBtn->setCheckable(true);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->addButton(ui->aliPayBtn, 0);
    m_buttonGroup->addButton(ui->wePayBtn, 1);
    m_buttonGroup->addButton(ui->cashBtn, 2);
    m_buttonGroup->setExclusive(true);

    connect(m_buttonGroup, &QButtonGroup::idClicked,
            this, &PaymentWidget::onBtn);
}

void PaymentWidget::onBtn(int btnId)
{
    m_buttonGroup->button(btnId)->setChecked(true);
}
