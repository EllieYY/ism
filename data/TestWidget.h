#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include "WidgetBase.h"
#include "logger.h"
#include "propertyconfigurator.h"
#include <QNetworkReply>

namespace Ui {
class TestWidget;
}


class LibcurlFtp;
class TestWidget : public WidgetBase
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit TestWidget(QWidget *parent = nullptr);
    ~TestWidget();

private slots:
    // AFC 接口测试
    void on_Btn3001_clicked();
    void on_Btn4002_clicked();
    void on_Btn7000_clicked();
//    void on_Btn7001_clicked();
    void on_Btn9003_login_clicked();
    void on_Btn9003_logout_clicked();
//    void on_Btn9002_clicked();
//    void on_Btn9002_2_clicked();

    // FTP文件上传下载
//    void on_pathBtn_clicked();
    void on_fileBtn_clicked();
    void on_pushButton_download_clicked();
    void on_pushButton_upload_clicked();

    void error(QNetworkReply::NetworkError error);

    // 参数文件解析
    void on_p1001btn_clicked();
    void on_p1004btn_clicked();
    void on_p2004btn_clicked();
    void on_p2002btn_clicked();

    void on_pushButton_pfile_clicked();

    void on_pushButton_fileList_clicked();
    void showFileList(QString fileName);

private:
    void init();
    void secEvent();
    int parseHead(QDataStream &stream);    // 文件头解析
    bool fileCheck(QByteArray array);   // 文件校验

private:
    QString m_userName;
    LibcurlFtp* m_curlFtp;
    QString m_filePath;

signals:
    void onCashboxChecking(bool);
    void onReader(bool);

private:
    Ui::TestWidget *ui;
};

#endif // TESTWIDGET_H
