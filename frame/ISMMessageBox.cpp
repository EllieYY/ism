#include "ISMMessageBox.h"
#include "ui_ISMMessageBox.h"
#include "MyHelper.h"

ISMMessageBox::ISMMessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ISMMessageBox)
{
    ui->setupUi(this);

    this->mousePressed = false;
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置窗体关闭时自动释放内存
    this->setAttribute(Qt::WA_DeleteOnClose);
    //关联关闭按钮
    connect(ui->btnOk, &QPushButton::clicked, this, &ISMMessageBox::onBtnOkClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &ISMMessageBox::close);
    //窗体居中显示
    MyHelper::FormInCenter(this);
}

ISMMessageBox::~ISMMessageBox()
{
    delete ui;
}


void ISMMessageBox::SetMessage(const QString &msg, int type)
{
    if (type == 0) {
        ui->labIcoMain->setStyleSheet("border-image: url(images/info.png);");
        ui->btnCancel->setVisible(false);
        ui->lab_Title->setText("提示");
    } else if (type == 1) {
        ui->labIcoMain->setStyleSheet("border-image: url(images/question.png);");
        ui->lab_Title->setText("询问");
    } else if (type == 2) {
        ui->labIcoMain->setStyleSheet("border-image: url(images/error.png);");
        ui->btnCancel->setVisible(false);
        ui->lab_Title->setText("错误");
    }

    ui->labInfo->setText(msg);
}

void ISMMessageBox::onBtnOkClicked()
{
    done(1);
    close();
}

void ISMMessageBox::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void ISMMessageBox::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void ISMMessageBox::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}
