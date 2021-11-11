/* CSQWidgetBase 中央窗口基类
 *
 */
#include "WidgetBase.h"
#include "WidgetMng.h"
#include "StatusBar.h"

WidgetBase::WidgetBase(QWidget *parent)  : QWidget(parent)
{
    m_id			= 0;
	m_preWidgetId	= 0;
	m_isShow		= false;
    for (int i = 0; i < MAX_DATA_TYPE_NUM; i++)
    {
        m_dataUpdateNum[i] = true;  //#保证首次进入界面, 无数据更新时，刷新界面
    }
}

WidgetBase::~WidgetBase()
{
}


//#01.秒驱动: used in WidgetMng::sec_event
void WidgetBase::secEvent()
{

}

//#02.数据变化通知 used in WidgetMng::data_update
void WidgetBase::dataUpdate(int paraId)
{
    if (paraId < 0 || paraId > MAX_DATA_TYPE_NUM)
        return;
    m_dataUpdateNum[paraId] = true;
}

//#03.刷新界面
bool WidgetBase::showData()
{
	return m_isShow;
}

//#04
void WidgetBase::setStyle()
{
}


//#05.显示窗口
void WidgetBase::showWgt()
{
	m_isShow = true;
	show();
    QApplication::setActiveWindow(this);
    showData();
}

//#06.关闭窗口
void WidgetBase::hideWgt()
{
	m_isShow = false;
	hide();
}


