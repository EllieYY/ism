#include "TicketUpdateTestDlg.h"
#include "ui_TicketUpdateTestDlg.h"

TicketUpdateTestDlg::TicketUpdateTestDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TicketUpdateTestDlg)
{
    ui->setupUi(this);
}

TicketUpdateTestDlg::~TicketUpdateTestDlg()
{
    delete ui;
}
