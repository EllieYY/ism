#include "ISMFrame.h"
#include <QFile>

#include "LogHandler.h"
#include "logger.h"
#include "propertyconfigurator.h"
#include "LoginDlg.h"
#include <iostream>
#include <QDebug>

/**
 * @Author: Ellie
 * @Description:
 **/

#include <QApplication>
#include "qtsingleapplication.h"
#include "qkeytools.h"

int main(int argc, char *argv[])
{
//    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
//    QApplication a(argc, argv);

    QtSingleApplication a("ism_id", argc, argv);
    if (a.isRunning()) {
        a.sendMessage("raise_window_noop", 4000);
        return EXIT_SUCCESS;
    }

//    Singleton<LogHandler>::getInstance().installMessageHandler();

    //配置文件路径
    Log4Qt::PropertyConfigurator::configure(a.applicationDirPath()
    + "/log4qt.conf");

    //样式应用
    QFile file("style.css");
    file.open(QFile::ReadOnly);
    QString qss = QLatin1String(file.readAll());
    a.setStyleSheet(qss);
//    a.setPalette(QPalette(QColor("#F0F0F0")));

    ISMFrame* w = new ISMFrame();
    a.setActivationWindow(w, 1);
    w->login();
//    w->show();
//    w->showMaximized();


#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    //QT5版本以上默认就是采用UTF-8编码
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
    // 设置键盘的字体大小
    QFont sfont;
    sfont.setPointSize(16);
    qApp->setFont(sfont);

    // 设置键盘的父对象、宽度、高度、以及默认的样式设置
//    QkeyTools::getInstance()->setMainWindowObject(&w);
    QkeyTools::getInstance()->setWidth(1200);
    QkeyTools::getInstance()->setHeight(400);
    QkeyTools::getInstance()->Init(QkeyTools::ScreenBottom, QkeyTools::GRAY, 16, 16);

    int ret = a.exec();
//    Singleton<LogHandler>::getInstance().uninstallMessageHandler();

    return ret;
}
