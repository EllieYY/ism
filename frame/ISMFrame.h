#ifndef ISMFRAME_H
#define ISMFRAME_H

#include <QFrame>
#include <QVBoxLayout>
#include <QDateTime>
#include <QThread>

namespace Ui {
class ISMFrame;
}


class WidgetBase;
class LoginDlg;
class DeviceManager;
class ReaderManager;
class CompensationFareWidget;
class CardReadWidget;
class ISMFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ISMFrame(QWidget *parent = nullptr);
    ~ISMFrame();

    void login();
    void logout();

signals:
    void initDeviceInThread();
    void deviceUpdate();           // 设备更新检查并重新自检

private:
    void init();
    void initShow();
    void initDevice();
    void initTimer();
    void onTimer();
    void secEvent();
    void initWgt();
    void registerWidget(QHBoxLayout* layout, WidgetBase* widget, int widgetId, bool isMain);

    void onServiceOff();     // 运营结束操作

private:
    QTimer*   m_time;
    QDateTime m_oldtime;
    LoginDlg* m_loginDlg;

    DeviceManager* m_deviceManager;
    ReaderManager* m_readerMng;
    QThread* m_deviceThread;

    CompensationFareWidget* m_fareWidget;
//    CardReadWidget* m_cardReadWidget;
private:
    Ui::ISMFrame *ui;
};

#endif // ISMFRAME_H
