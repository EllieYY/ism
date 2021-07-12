#include "LineInfo.h"

LineInfo::LineInfo(QString name, QString picPath,
                   QString color, QObject *parent) : QObject(parent)
{
    m_name = name;
    m_picPath = picPath;
    m_color = color;
}



QString LineInfo::getName() const
{
    return m_name;
}

void LineInfo::setName(const QString &name)
{
    m_name = name;
}

QString LineInfo::getPicPath() const
{
    return m_picPath;
}

void LineInfo::setPicPath(const QString &picPath)
{
    m_picPath = picPath;
}

QString LineInfo::getColor() const
{
    return m_color;
}

void LineInfo::setColor(const QString &color)
{
    m_color = color;
}
