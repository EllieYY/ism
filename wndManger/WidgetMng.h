#pragma once
#include <vector>

#include <QObject>

class WidgetBase;

class WidgetMng : public QObject
{
    Q_OBJECT

public:
    WidgetMng(QObject *parent = nullptr);
    virtual ~WidgetMng(void);
public:
    static WidgetMng* getThis();
	static void notify(int para_id);
    void dataUpdate(int para_id);

    void showWidget(int wgtId,bool isReturn = false);
    void insertWidget(WidgetBase* wgt);
    void setMainWidget(WidgetBase* wgt);

signals:
    void returnCtl(bool isMainWnd);

public slots:
    void onReturn();
    void onHome();
public:
    void secEvent();
private:
	std::vector<WidgetBase*> m_wgtList;        //#窗口列表
	WidgetBase*              m_currentWgt;     //#当前显示的窗口
    WidgetBase*              m_mainWgt;		   //#主窗口
};
