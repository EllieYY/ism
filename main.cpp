#include "ISMFrame.h"
#include <QFile>

#include "LogHandler.h"
#include "logger.h"
#include "propertyconfigurator.h"

/**
 * @Author: Ellie
 * @Description:
 **/

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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


    ISMFrame w;
    w.show();

    int ret = a.exec();
//    Singleton<LogHandler>::getInstance().uninstallMessageHandler();

    return ret;
}
