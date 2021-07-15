#ifndef WIDGETBASE_H
#define WIDGETBASE_H

#include "CommonHead.h"


#define MAX_DATA_TYPE_NUM	7

class WidgetBase : public QWidget
{
	Q_OBJECT
public:
    WidgetBase(QWidget *parent);
    virtual ~WidgetBase();
public:
    virtual void secEvent();
    virtual void dataUpdate(int para_id);
    virtual bool showData();
	virtual void setStyle();
    virtual void onStationSelected(QString lineName, QString stationName, QString stationCode);
public:
    virtual void showWgt();
    virtual void hideWgt();

public:
	inline void setPreWgt(int preWgtId)	{ m_preWidgetId = preWgtId;}
    inline int getPreWgt()	{ return m_preWidgetId;}
	inline bool is_show()				{ return m_isShow;}
	inline int  getId()					{ return m_id;};
    inline void setId(int id)			{ m_id = id;};

private:	//#上一个窗口
    bool                    m_isShow;			//#显示控制
protected:
    int                     m_id;				//#窗口id
    int                     m_preWidgetId;		//#上一个窗口
    bool					m_dataUpdateNum[MAX_DATA_TYPE_NUM];


signals:
    void selectStation();    // 选择车站

};

#endif // WIDGETBASE_H
