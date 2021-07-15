#include "Station.h"

Station::Station(QString name, QString code,QObject *parent) : QObject(parent)
{
    m_name = name;
    m_code = code;
}

QString Station::name() const
{
    return m_name;
}

void Station::setName(const QString &name)
{
    m_name = name;
}

QString Station::code() const
{
    return m_code;
}

void Station::setCode(const QString &code)
{
    m_code = code;
}
