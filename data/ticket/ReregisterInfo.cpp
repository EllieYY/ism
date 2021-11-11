#include "ReregisterInfo.h"

ReregisterInfo::ReregisterInfo(QObject *parent) : QObject(parent)
{
//    m_type = type;
//    m_state = state;
//    m_enterStation = enterStation;
//    m_enterTime = enterTime;
//    m_exitStation = exitStation;
//    m_exitTime = exitTime;
//    m_validDate = validDate;
//    m_arrearage = arrearage;
//    m_reason = reason;
}

int ReregisterInfo::type() const
{
    return m_type;
}

void ReregisterInfo::setType(int type)
{
    m_type = type;
}

int ReregisterInfo::state() const
{
    return m_state;
}

void ReregisterInfo::setState(int state)
{
    m_state = state;
}

QString ReregisterInfo::enterStation() const
{
    return m_enterStation;
}

void ReregisterInfo::setEnterStation(const QString &enterStation)
{
    m_enterStation = enterStation;
}

QDateTime ReregisterInfo::enterTime() const
{
    return m_enterTime;
}

void ReregisterInfo::setEnterTime(const QDateTime &enterTime)
{
    m_enterTime = enterTime;
}

QString ReregisterInfo::exitStation() const
{
    return m_exitStation;
}

void ReregisterInfo::setExitStation(const QString &exitStation)
{
    m_exitStation = exitStation;
}

QDateTime ReregisterInfo::exitTime() const
{
    return m_exitTime;
}

void ReregisterInfo::setExitTime(const QDateTime &exitTime)
{
    m_exitTime = exitTime;
}

QDate ReregisterInfo::validDate() const
{
    return m_validDate;
}

void ReregisterInfo::setValidDate(const QDate &validDate)
{
    m_validDate = validDate;
}


QString ReregisterInfo::reason() const
{
    return m_reason;
}

void ReregisterInfo::setReason(const QString &reason)
{
    m_reason = reason;
}

QString ReregisterInfo::number() const
{
    return m_number;
}

void ReregisterInfo::setNumber(const QString &number)
{
    m_number = number;
}

QDate ReregisterInfo::createTime() const
{
    return m_createTime;
}

void ReregisterInfo::setCreateTime(const QDate &createTime)
{
    m_createTime = createTime;
}

float ReregisterInfo::balance() const
{
    return m_balance;
}

void ReregisterInfo::setBalance(float balance)
{
    m_balance = balance;
}

int ReregisterInfo::reregisterType() const
{
    return m_reregisterType;
}

void ReregisterInfo::setReregisterType(int reregisterType)
{
    m_reregisterType = reregisterType;
}

