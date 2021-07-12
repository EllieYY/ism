#include "CustomTabWidget.h"
#include <QLabel>
#include <QTabBar>

CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent)
{
    connect(this,&QTabWidget::tabBarClicked,this,&CustomTabWidget::slotSetLabelColor);
}

void CustomTabWidget::slotSetLabelColor(int index)
{
    for(int i=0; i<this->count(); i++)
    {
        this->tabBar()->tabButton(i,QTabBar::ButtonPosition::RightSide)->
                setStyleSheet("background-color: #F3FBF7; color: #09262A;");
    }
    this->tabBar()->tabButton(index,QTabBar::ButtonPosition::RightSide)->
            setStyleSheet("background-color: #E0F4EB; color: #7B8081;");

}

int CustomTabWidget::addTab(QWidget *widget, const QString &label)
{
    int nRetrun = QTabWidget::addTab(widget,"");
    int count = this->count() - 1;
    QLabel *labelTab = new QLabel(label,this);
    QFont ft;
    ft.setPointSize(20);
    labelTab->setFont(ft);
    labelTab->setAlignment(Qt::AlignCenter);
    this->tabBar()->setTabButton(count,QTabBar::ButtonPosition::RightSide,labelTab);
    this->tabBar()->tabButton(count,QTabBar::ButtonPosition::RightSide)->setFixedHeight(64);
    updateselect();
    return nRetrun;

}

void CustomTabWidget::updateselect()
{
    for(int i = 0; i < this->currentIndex(); i++)
    {
        this->tabBar()->tabButton(i,QTabBar::ButtonPosition::RightSide)->
                setStyleSheet("background-color: #F3FBF7; color: #09262A; font-size: 32px;");
    }
    this->tabBar()->tabButton(this->currentIndex(),QTabBar::ButtonPosition::RightSide)->
            setStyleSheet("background-color: #E0F4EB; color: #7B8081;");

}

