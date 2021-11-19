#include "TestWidget.h"
#include "ui_TestWidget.h"
#include "DataCenter.h"
#include "NCNetwork_Lib.h"
#include "MyHelper.h"

TestWidget::TestWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);

    init();
}

TestWidget::~TestWidget()
{
    delete ui;
}


void TestWidget::init()
{
    m_userName = "123445";

}

void TestWidget::secEvent()
{

}

void TestWidget::on_Btn3001_clicked()
{
    DataCenter::getThis()->deviceState2afc();
}

void TestWidget::on_Btn4002_clicked()
{
    DataCenter::getThis()->param2afc();
}

void TestWidget::on_Btn7000_clicked()
{
    QString fileNameStr = "Y.04200403.20211118102930.000001";
    QByteArray fileNamedata = fileNameStr.toUtf8();
    QString md5Str = "20211101111253202111011112535353";

    BYTE* fileName = reinterpret_cast<byte*>(fileNameStr.toLocal8Bit().data());
    BYTE* md5 = reinterpret_cast<byte*>(md5Str.toLocal8Bit().data());

    BYTE ret = FileDownloadNotify(0x03, (byte*)fileNamedata.data(), md5);
    logger()->info("文件传输通知7000 = {%1}", ret);
}

void TestWidget::on_Btn7001_clicked()
{
//    FileDownloadConfirm(0x03, BYTE *fileName, 0x00);
}

void TestWidget::on_Btn9003_login_clicked()
{
    // 登录
    QString userName = m_userName;
    BYTE * operatorID = reinterpret_cast<byte*>(userName.toLocal8Bit().data());
    BYTE event = 2;          // 带口令登录
    BYTE operatorType = 2;   // 维护人员
    BYTE ret = OperatorAction(operatorID, event, operatorType);
    if (ret != 0) {
        MyHelper::ShowMessageBoxError(QString("登录失败[%1]，请联系工作人员。").arg(ret));
        logger()->error("AFC登录失败{%1}", ret);
        return;
    }
}

void TestWidget::on_Btn9003_logout_clicked()
{
    // AFC 签退
    QString userName = m_userName;
    BYTE * operatorID = reinterpret_cast<byte*>(userName.toLocal8Bit().data());
    BYTE event = 0;          // 签退
    BYTE operatorType = 2;   // 维护人员
    BYTE ret = OperatorAction(operatorID, event, operatorType);
    if (ret != 0) {
        MyHelper::ShowMessageBoxError(QString("签退失败[%1]，请联系工作人员。").arg(ret));
        return;
    }

}

void TestWidget::on_Btn9002_clicked()
{
    DataCenter::getThis()->samInfo2afc();
}

void TestWidget::on_Btn9002_2_clicked()
{

}
