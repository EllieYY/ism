#ifndef TICKETUPDATETESTDLG_H
#define TICKETUPDATETESTDLG_H

#include <QWidget>

namespace Ui {
class TicketUpdateTestDlg;
}

class TicketUpdateTestDlg : public QWidget
{
    Q_OBJECT

public:
    explicit TicketUpdateTestDlg(QWidget *parent = nullptr);
    ~TicketUpdateTestDlg();

private:
    Ui::TicketUpdateTestDlg *ui;
};

#endif // TICKETUPDATETESTDLG_H
