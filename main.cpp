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








    int ret = a.exec();
//    Singleton<LogHandler>::getInstance().uninstallMessageHandler();

    return ret;
}
