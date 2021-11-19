#ifndef ISMWAITING_H
#define ISMWAITING_H

#include <QDialog>

namespace Ui {
class ISMWaiting;
}

class QMovie;
class ISMWaiting : public QDialog
{
    Q_OBJECT

public:
    explicit ISMWaiting(QWidget *parent = nullptr);
    ~ISMWaiting();

private:
    QMovie*  m_movie;

private:
    Ui::ISMWaiting *ui;
};

#endif // ISMWAITING_H
