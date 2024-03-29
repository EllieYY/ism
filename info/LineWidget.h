﻿#ifndef LINEWIDGET_H
#define LINEWIDGET_H

#include <QWidget>
#include "WidgetBase.h"

class LineInfo;
namespace Ui {
class LineWidget;
}

class LineWidget : public WidgetBase
{
    Q_OBJECT

public:
    explicit LineWidget(QWidget *parent = nullptr);
    ~LineWidget();

    void onReadLines();
    void onLineQuery(QString);


private:
    void init();
    void onLineChange(int id);
    bool showData();

    void lineQuery();

private slots:
    void onStartLineSelect(QString data);
    void onEndLineSelect(QString data);

    void showImage(QString lineImagePath);

private:
    QList<LineInfo*>        m_lineInfoLst;
    QButtonGroup*           m_btnGroup;
    QVBoxLayout*            m_layout;

private:
    Ui::LineWidget *ui;
};

#endif // LINEWIDGET_H
