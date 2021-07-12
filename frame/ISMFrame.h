#ifndef ISMFRAME_H
#define ISMFRAME_H

#include <QFrame>
#include <QVBoxLayout>
#include <QDateTime>

namespace Ui {
class ISMFrame;
}

class StationSelectWidget;
class WidgetBase;
class ISMFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ISMFrame(QWidget *parent = nullptr);
    ~ISMFrame();

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
    StationSelectWidget* m_stationSelectedWidget;

private:
    Ui::ISMFrame *ui;
};

#endif // ISMFRAME_H
