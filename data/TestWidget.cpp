#include "TestWidget.h"
#include "ui_TestWidget.h"
#include "DataCenter.h"
#include "NCNetwork_Lib.h"
#include "MyHelper.h"
#include "FtpManager.h"
#include <QFileDialog>
#include "curl.h"
#include <iostream>
#include <sys/stat.h>
#include "LibcurlFtp.h"


TestWidget::TestWidget(QWidget *parent) :
    WidgetBase(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);
    init();
}

TestWidget::~TestWidget()
{
    if (m_curlFtp != NULL) {
        delete m_curlFtp;
        m_curlFtp = NULL;
    }

    if (m_ftp != NULL) {
        delete m_ftp;
        m_ftp = NULL;
    }
    delete ui;
}


void TestWidget::init()
{
    m_userName = "123445";
    m_ftp = new FtpManager(this);
    m_ftp->setHostPort("192.168.2.193", 21);
    m_ftp->setUserInfo("ismftp", "1234Asdf");

    m_curlFtp = new LibcurlFtp(this);
    m_curlFtp->setHostPort("192.168.2.193", 21);
    m_curlFtp->setUserInfo("ismftp", "1234Asdf");
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


// FTP接口测试
void TestWidget::on_fileBtn_clicked()
{
    // /ftpdata/ismftp/
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择上传文件"), "D:/", tr("All files(*.*)"));
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "文件选取失败!");
        return;
    }

    m_filePath = fileName;
    ui->lineEdit_upload->setText(fileName);
}

void TestWidget::on_pushButton_download_clicked()
{
    QString localPath = "ism-net";
    QString serverPath = "folder1/";
    m_curlFtp->ftpDownload(localPath, serverPath);

    qDebug() << "dowmload……";
//    m_ftp->get("/root/set-net.sh", "D:\\set-net.sh");
//    connect(m_ftp, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
//    connect(m_ftp, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
}

void TestWidget::on_pushButton_upload_clicked()
{
    QString localPath = "D:\\test.txt";
    QString serverPath = "folder1/test.txt";
    m_curlFtp->ftpDownload(localPath, serverPath);

//    qDebug() << "==== upload test ====";
//    FtpUpload("ftp://192.168.2.193:21/ftpdata/ismftp/set1.sh", "D:\\set-net.sh", "ismftp", "1234Asdf", 100000);


    // Qt5 方法，只能上传和下载
//    qDebug() << "upload……";
////    QString file = ui->lineEdit_upload->text();
//    m_ftp->put("D:/test.txt", "/test.txt");
//    connect(m_ftp, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
//    connect(m_ftp, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(uploadProgress(qint64, qint64)));
}


// 错误处理
void TestWidget::error(QNetworkReply::NetworkError error)
{
    qDebug() << "error" << error;
    switch (error) {
    case QNetworkReply::HostNotFoundError :
        qDebug() << QString::fromLocal8Bit("主机没有找到");
        break;
        // 其他错误处理
    default:
        break;
    }
}

void TestWidget::on_pathBtn_clicked()
{

}


void TestWidget::on_pushButton_fileList_clicked()
{
//    QByteArray ba = m_fileName.toLocal8Bit();
//    const char *c_str = ba.constData();

    QString url  = ui->lineEdit_url->text();
    QString localPath = "ism-net";
    QString serverPath = "folder1/";
    m_curlFtp->ftpDownload(localPath, serverPath);
    connect(m_curlFtp, &LibcurlFtp::downloadOk, this, &TestWidget::showFileList);
}

void TestWidget::showFileList(QString fileName)
{
    QFile file(fileName);
    bool isOk = file.open(QFile::ReadOnly);
    if (!isOk) {
        QMessageBox::critical(this,"ERROR","file open failed");
        return;
    }

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        QDataStream out(line);

        QString access;
        int num;
        int owner;
        int group;
        QString datetime;
        QString filename;
        out << access;
        out << num;
        out << owner;
        out << group;
        out << datetime;
        out << fileName;

        logger()->info("%1 %2 %3 %4 %5 %6", access, num, owner, group, datetime, fileName);

//        qDebug() << str;
    }


    file.close();
}


// 参数文件解析

int TestWidget::parseHead(QDataStream &stream)
{
    // 文件头
    qint8 type;
    QByteArray buffer(7, Qt::Uninitialized);
    qint8 version;
    qint32 count = 0;
    qint16 paramType;

    stream >> type;
    stream.readRawData(buffer.data(), 7);
    stream >> version;
    stream >> count;
    stream >> paramType;

    QString timeStr(buffer.toHex());
    QString head = QString("type={%1}, time={%2}, version={%3}, count={%4}, pType={%5},")
            .arg(type, 2, 16, QLatin1Char('0'))
            .arg(timeStr)
            .arg(version, 2, 16, QLatin1Char('0'))
            .arg(count, 8, 16, QLatin1Char('0'))
            .arg(paramType, 4, 16, QLatin1Char('0'));
    qDebug() << "head : " << head;

    return count;
}


// 文件校验：只计算md5
bool TestWidget::fileCheck(QByteArray array)
{
    int size = array.size();
    // 获取16个字节的md5
    QByteArray md5Array = array.right(16);
    QByteArray body = array.left(size - 16);

    QString bodyMD5Str = QCryptographicHash::hash(body, QCryptographicHash::Md5).toHex();
    QString md5Str = md5Array.toHex();

    if (md5Str.compare(bodyMD5Str) == 0) {
        return true;
    }

    return false;
}


void TestWidget::on_p1001btn_clicked()
{
    QString fileName = ui->lineEdit_param->text();
    qDebug() << "param file name: " << fileName;

    QFile file(fileName);
    bool isOk = file.open(QFile::ReadOnly);
    if (!isOk) {
        QMessageBox::critical(this,"ERROR","file open failed");
        return;
    }

    QDataStream stream(&file);
    parseHead(stream);

    // 文件内容
    qint32 pVersion;
    QByteArray startTime(7, Qt::Uninitialized);
    BYTE tradeCountL;
    BYTE tradeTimeH;
    BYTE deviceInterval;

    stream >> pVersion;
    stream.readRawData(startTime.data(), 7);
    stream.skipRawData(7);
    stream >> tradeCountL;
    stream >> tradeTimeH;
    stream >> deviceInterval;

    QString sTimeStr(startTime.toHex());
    QString body = QString("pVersion={%1}, startTime={%2}, tradeCountL={%3}, tradeTimeH={%4}, deviceInterval={%5},")
            .arg(pVersion, 8, 16, QLatin1Char('0'))
            .arg(sTimeStr)
            .arg(tradeCountL, 2, 16, QLatin1Char('0'))
            .arg(tradeTimeH, 2, 16, QLatin1Char('0'))
            .arg(deviceInterval, 2, 16, QLatin1Char('0'));
    qDebug() << "body : " << body;

    file.close();

}

// 车票类型表
void TestWidget::on_p1004btn_clicked()
{
    QString fileName = ui->lineEdit_param->text();
    qDebug() << "param file name: " << fileName;

    QFile file(fileName);
    bool isOk = file.open(QFile::ReadOnly);
    if (!isOk) {
        QMessageBox::critical(this,"ERROR","file open failed");
        return;
    }

    QByteArray array = file.readAll();
    bool checkOk = fileCheck(array);
    qDebug() << "file check: " << checkOk;

    QDataStream stream(&file);
    int count = parseHead(stream);

    qint32 version;
    QByteArray startTime(7, Qt::Uninitialized);

    stream >> version;
    stream.readRawData(startTime.data(), 7);
    stream.skipRawData(3);
    qDebug() << "version: " << version;
    qDebug() << "startTime: " << QString(startTime.toHex());

    for (int i = 0; i < count; i++) {
        stream.skipRawData(1);
        BYTE code;
        stream >> code;
        QByteArray en(15, Qt::Uninitialized);
        QByteArray ch(15, Qt::Uninitialized);
        stream.readRawData(en.data(), 15);
        stream.readRawData(ch.data(), 15);

        QString info = QString("code={%1},en={%2},ch={%3}")
                .arg(code, 2, 16, QLatin1Char('0'))
                .arg(QString(en.toHex()))
                .arg(QString(ch.toHex()));

        qDebug() << info;

        QString enStr = MyHelper::getCorrectUnicode(en);
        QString chStr = MyHelper::getCorrectUnicode(ch);
        qDebug() << enStr;
        qDebug() << chStr;

        stream.skipRawData(193);
    }

    file.close();
}


// 操作员列表
void TestWidget::on_p2004btn_clicked()
{
    QString fileName = ui->lineEdit_param->text();
    qDebug() << "param file name: " << fileName;

    QFile file(fileName);
    bool isOk = file.open(QFile::ReadOnly);
    if (!isOk) {
        QMessageBox::critical(this,"ERROR","file open failed");
        return;
    }

    QDataStream stream(&file);
    int m = parseHead(stream);

    qint32 version;    // 4 byte
    QByteArray buffer(7, Qt::Uninitialized);
    stream >> version;
    stream.readRawData(buffer.data(), 7);

    // 操作员信息
    for (int i = 0; i < m; i++) {
        QByteArray code(4, Qt::Uninitialized);
        QByteArray name(32, Qt::Uninitialized);
        QByteArray card(6, Qt::Uninitialized);
        QByteArray pwd(8, Qt::Uninitialized);
        BYTE type;
        BYTE deviceAccess;
        QByteArray validDate(7, Qt::Uninitialized);
        qint32 n;
        QByteArray stateCode(4, Qt::Uninitialized);

        stream.readRawData(code.data(), 4);
        stream.readRawData(name.data(), 32);
        stream.readRawData(card.data(), 6);
        stream.readRawData(pwd.data(), 8);
        stream >> type;
        stream >> deviceAccess;
        stream.readRawData(validDate.data(), 7);
        stream >> n;

        // 可操作车站编码
        stream.skipRawData(n * 4);

        // 保留位
        stream.skipRawData(8);

        QString nameStr = MyHelper::getCorrectUnicode(name);
        logger()->info("[param2004]code={%1}, name={%2}, card={%3}, pwd={%4}, type={%5}, access={%6}, validDate={%7}",
                       QString(code.toHex()),
                       nameStr,
                       QString(card.toHex()),
                       QString(pwd.toHex()),
                       QString::number(type, 16),
                       QString::number(deviceAccess, 16),
                       QString(validDate.toHex()));
    }

    file.close();
}

void TestWidget::on_p2002btn_clicked()
{
    QString fileName = ui->lineEdit_param->text();
    qDebug() << "param file name: " << fileName;

    QFile file(fileName);
    bool isOk = file.open(QFile::ReadOnly);
    if (!isOk) {
        QMessageBox::critical(this,"ERROR","file open failed");
        return;
    }

    QDataStream stream(&file);
    parseHead(stream);

    qint32 version;
    QByteArray startTime(7, Qt::Uninitialized);
    qint16 maxCount;

    stream >> version;
    stream.readRawData(startTime.data(), 7);
    stream.skipRawData(109);
    stream >> maxCount;

    logger()->info("[param2002]version=%1, time=%2, maxCount=%3", version, QString(startTime.toHex()), maxCount);


    file.close();
}

void TestWidget::on_pushButton_pfile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择参数文件"), "D://02_ISM//BOM_param/", tr("All files(*.*)"));
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "文件选取失败!");
        return;
    }

    ui->lineEdit_param->setText(fileName);
}


