#include "ISMWaiting.h"
#include "ui_ISMWaiting.h"
#include <QMovie>

ISMWaiting::ISMWaiting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ISMWaiting)
{
    ui->setupUi(this);
    //取消对话框标题
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowModality(Qt::WindowModal);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose);

    ui->label->setStyleSheet("background-color: transparent;");
    m_movie = new QMovie("images/home/waiting.gif");
    ui->label->setMovie(m_movie);
    ui->label->setScaledContents(true);
    m_movie->start();
}

ISMWaiting::~ISMWaiting()
{
    m_movie->stop();
    delete ui;
}
