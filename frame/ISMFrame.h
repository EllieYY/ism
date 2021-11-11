#ifndef ISMFRAME_H
#define ISMFRAME_H

#include <QFrame>
#include <QVBoxLayout>
#include <QDateTime>

namespace Ui {
class ISMFrame;
}


class WidgetBase;
class LoginDlg;
class ISMFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ISMFrame(QWidget *parent = nullptr);
    ~ISMFrame();

    void login();
    void logout();

private:
    void init();
    void initTimer();
    void onTimer();
    void secEvent();
    void initWgt();
    void registerWidget(QHBoxLayout* layout, WidgetBase* widget, int widgetId, bool isMain);

private:
    QTimer*   m_time;
    QDateTime m_oldtime;
    LoginDlg* m_loginDlg;

private:
    Ui::ISMFrame *ui;
};

#endif // ISMFRAME_H
