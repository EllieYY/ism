#include "TicketInfo.h"

TicketInfo::TicketInfo(QString type, QString number, QDate createTime, QDate validDate,
                       int cardState, int tripState, float balance, QObject *parent) : QObject(parent)
{
    m_type = type;
    m_number = number;
    m_createTime = createTime;
    m_validDate = validDate;
    m_cardState = cardState;
    m_tripState = tripState;
    m_balance = balance;
}

QString TicketInfo::type() const
{
    return m_type;
}

void TicketInfo::setType(const QString &type)
{
    m_type = type;
}

QString TicketInfo::number() const
{
    return m_number;
}

void TicketInfo::setNumber(const QString &number)
{
    m_number = number;
}

QDate TicketInfo::createTime() const
{
    return m_createTime;
}

void TicketInfo::setCreateTime(const QDate &createTime)
{
    m_createTime = createTime;
}

QDate TicketInfo::validDate() const
{
    return m_validDate;
}

void TicketInfo::setValidDate(const QDate &validDate)
{
    m_validDate = validDate;
}

int TicketInfo::cardState() const
{
    return m_cardState;
}

void TicketInfo::setCardState(int cardState)
{
    m_cardState = cardState;
}

int TicketInfo::tripState() const
{
    return m_tripState;
}

void TicketInfo::setTripState(int tripState)
{
    m_tripState = tripState;
}

float TicketInfo::balance() const
{
    return m_balance;
}

void TicketInfo::setBalance(float balance)
{
    m_balance = balance;
}
