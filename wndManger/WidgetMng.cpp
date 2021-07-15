/*
 *class CSQWidgetMng 
 *1. 在CSQFrameDlg::initWgt()中实例化
 *2. 实例化CSQFocusManager
 */
#include "WidgetMng.h"
#include "WidgetBase.h"

static WidgetMng* afx_this = NULL;

WidgetMng::WidgetMng(QObject *parent) :QObject(parent)
{
	afx_this	 = this;
	m_currentWgt = NULL;
	m_mainWgt	 = NULL;
    m_wgtList.clear();
}

WidgetMng::~WidgetMng(void)
{
	afx_this = NULL;
	m_wgtList.clear();
}

WidgetMng* WidgetMng::getThis()
{
	return afx_this;
}

/*STATIC used DataCenter::setParam*/
void WidgetMng::notify(int para_id)
{
    if (WidgetMng::getThis() != NULL)
        WidgetMng::getThis()->dataUpdate(para_id);
}


void WidgetMng::dataUpdate(int para_id)
{
    for (unsigned int i = 0;i < m_wgtList.size(); i++)
    {
        m_wgtList[i]->dataUpdate(para_id);
    }
}


/*注册窗口: used in CSQFrameDlg::initWgt()*/
void WidgetMng::insertWidget(WidgetBase* wgt)
{
	m_wgtList.push_back(wgt);
}

//#窗口切换
void WidgetMng::showWidget(int wgtId,bool isReturn/*default false*/)
{
	unsigned int i = 0;
	for (i = 0;i < m_wgtList.size(); i++)
		m_wgtList[i]->hideWgt();

	for (i = 0;i < m_wgtList.size(); i++)
	{
		if (m_wgtList[i]->getId() == wgtId)
		{
			if (!isReturn && m_currentWgt)
			{
				m_wgtList[i]->setPreWgt(m_currentWgt->getId());
			}

            m_wgtList[i]->showWgt();
			m_currentWgt = m_wgtList[i];	//# 

            bool isMainWnd = (wgtId == MAIN_DLG);
            emit returnCtl(isMainWnd);
			break;
		}
    }
}

void WidgetMng::onReturn()
{
    if (!m_currentWgt) {
        return;
    }

    WidgetMng::getThis()->showWidget(m_currentWgt->getPreWgt(), true);
}

void WidgetMng::onHome()
{
    WidgetMng::getThis()->showWidget(MAIN_DLG, false);
}


void WidgetMng::setMainWidget(WidgetBase* wgt)
{
	m_mainWgt = wgt;
}

/*秒驱动: used in  ISMFrameDlg::secEvent*/
void WidgetMng::secEvent()
{
	for (unsigned int i = 0;i < m_wgtList.size(); i++)
	{
		if (m_wgtList[i]->is_show())
            m_wgtList[i]->secEvent();
	}
}


