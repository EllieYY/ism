#ifndef MYHELPER_H
#define MYHELPER_H

#include <QtCore>
#include <QtGui>
#include <QDesktopWidget>
#include "ISMMessageBox.h"

class MyHelper: public QObject
{

public:
    //
    static void AutoRunWithSystem(bool IsAutoRun, QString AppName, QString AppPath)
    {
        QSettings *reg = new QSettings(
            "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
            QSettings::NativeFormat);

        if (IsAutoRun) {
            reg->setValue(AppName, AppPath);
        } else {
            reg->setValue(AppName, "");
        }
    }

    // 中文乱码处理
    static void SetUTF8Code()
    {
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
#endif
    }

//    // 样式生效
//    static void SetStyle(const QString &styleName)
//    {
//        QFile file(QString(":/image/%1.css").arg(styleName));
//        file.open(QFile::ReadOnly);
//        QString qss = QLatin1String(file.readAll());
//        qApp->setStyleSheet(qss);
//        qApp->setPalette(QPalette(QColor("#F0F0F0")));
//    }

    // 中文设置
    static void SetChinese()
    {
        QTranslator *translator = new QTranslator(qApp);
        translator->load(":/image/qt_zh_CN.qm");
        qApp->installTranslator(translator);
    }

    // IP地址判断
    static bool IsIP(QString IP)
    {
        QRegExp RegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
        return RegExp.exactMatch(IP);
    }

    // 信息提示框
    static void ShowMessageBoxInfo(QString info)
    {
        ISMMessageBox *msg = new ISMMessageBox;
        FormInCenter(msg);
        msg->SetMessage(info, 0);
        msg->exec();
    }

    // 错误框：确认按钮
    static void ShowMessageBoxError(QString info)
    {
        ISMMessageBox *msg = new ISMMessageBox;
        FormInCenter(msg);
        msg->SetMessage(info, 2);
        msg->exec();
    }

    // 询问框：确认和取消按钮
    static int ShowMessageBoxQuesion(QString info)
    {
        ISMMessageBox *msg = new ISMMessageBox;
        FormInCenter(msg);
        msg->SetMessage(info, 1);
        return msg->exec();
    }

    // 延时
    static void Sleep(int sec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(sec);
        while ( QTime::currentTime() < dieTime ) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    // 窗体居中
    static void FormInCenter(QWidget *frm)
    {
        int frmX = frm->width();
        int frmY = frm->height();
        QDesktopWidget w;
        int deskWidth = w.width();
        int deskHeight = w.height();
        QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
        frm->move(movePoint);
    }

    // 字符转16进制 "12aB" -> 0x12AB
    static QByteArray hexStrToByte(QString str)
    {
        QByteArray byteArray;
        bool ok;
        if(str.size() % 2 != 0){
            return QByteArray::fromHex("字符串不符合标准");
        }
        int len = str.size();
        for(int i = 0; i < len; i += 2) {
            byteArray.append(str.mid(i,2).toUpper().toUShort(&ok,16));
        }
        return byteArray;
    }

    // 字符转16进制 "12aB" -> 0x12AB
    static void hexStrToByte(QString str, int len, BYTE* des) {
        QByteArray byteArray = MyHelper::hexStrToByte(str);
        BYTE* desByte = (BYTE*)byteArray.data();
        memcpy(des, desByte, len);
    }

    // int转字节数字（网络序：高位在前）
    static void intToBigEndianByte(int src, int len, BYTE* des)
    {
        for(int i = 0; i < len; i++)
        {
            des[i] = (src >> (8 * (len - i - 1))) & 0xFF;
        }
    }

};

#endif // MYHELPER_H
