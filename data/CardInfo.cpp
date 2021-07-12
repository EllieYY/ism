#include "CardInfo.h"

CardInfo::CardInfo(int type, int state,
                   QString enterStation, QDateTime enterTime,
                   QString exitStation, QDateTime exitTime,
                   QDate validDate, float arrearage, QString reason,
                   QObject *parent) : QObject(parent)
{
    m_type = type;
    m_state = state;
    m_enterStation = enterStation;
    m_enterTime = enterTime;
    m_exitStation = exitStation;
    m_exitTime = exitTime;
    m_validDate = validDate;
    m_arrearage = arrearage;
    m_reason = reason;
}

int CardInfo::type() const
{
    return m_type;
}

void CardInfo::setType(int type)
{
    m_type = type;
}

int CardInfo::state() const
{
    return m_state;
}

void CardInfo::setState(int state)
{
    m_state = state;
}

QString CardInfo::enterStation() const
{
    return m_enterStation;
}

void CardInfo::setEnterStation(const QString &enterStation)
{
    m_enterStation = enterStation;
}

QDateTime CardInfo::enterTime() const
{
    return m_enterTime;
}

void CardInfo::setEnterTime(const QDateTime &enterTime)
{
    m_enterTime = enterTime;
}

QString CardInfo::exitStation() const
{
    return m_exitStation;
}

void CardInfo::setExitStation(const QString &exitStation)
{
    m_exitStation = exitStation;
}

QDateTime CardInfo::exitTime() const
{
    return m_exitTime;
}

void CardInfo::setExitTime(const QDateTime &exitTime)
{
    m_exitTime = exitTime;
}

QDate CardInfo::validDate() const
{
    return m_validDate;
}

void CardInfo::setValidDate(const QDate &validDate)
{
    m_validDate = validDate;
}

float CardInfo::arrearage() const
{
    return m_arrearage;
}

void CardInfo::setArrearage(float arrearage)
{
    m_arrearage = arrearage;
}

QString CardInfo::reason() const
{
    return m_reason;
}

void CardInfo::setReason(const QString &reason)
{
    m_reason = reason;
}
