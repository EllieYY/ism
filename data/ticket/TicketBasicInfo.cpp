#include "TicketBasicInfo.h"

TicketBasicInfo::TicketBasicInfo(int type, QString typeStr, QString number, QString createTime, QString validDate,
                       int cardState, int tripState, long balance, QObject *parent) : QObject(parent)
{
    m_typeNum = type;
    m_type = typeStr;
    m_number = number;
    m_createTime = QDate::fromString(createTime, "yyyyMMdd");
    m_validDate = QDate::fromString(validDate, "yyyyMMdd");;
    m_cardState = cardState;
    m_tripState = tripState;
    m_balance = balance * 0.01;
}

QString TicketBasicInfo::type() const
{
    return m_type;
}

void TicketBasicInfo::setType(const QString &type)
{
    m_type = type;
}

QString TicketBasicInfo::number() const
{
    return m_number;
}

void TicketBasicInfo::setNumber(const QString &number)
{
    m_number = number;
}

QDate TicketBasicInfo::createTime() const
{
    return m_createTime;
}

void TicketBasicInfo::setCreateTime(const QDate &createTime)
{
    m_createTime = createTime;
}

QDate TicketBasicInfo::validDate() const
{
    return m_validDate;
}

void TicketBasicInfo::setValidDate(const QDate &validDate)
{
    m_validDate = validDate;
}

int TicketBasicInfo::cardState() const
{
    return m_cardState;
}

void TicketBasicInfo::setCardState(int cardState)
{
    m_cardState = cardState;
}

int TicketBasicInfo::tripState() const
{
    return m_tripState;
}

void TicketBasicInfo::setTripState(int tripState)
{
    m_tripState = tripState;
}

float TicketBasicInfo::balance() const
{
    return m_balance;
}

void TicketBasicInfo::setBalance(float balance)
{
    m_balance = balance;
}

int TicketBasicInfo::typeNum() const
{
    return m_typeNum;
}

void TicketBasicInfo::setTypeNum(int typeNum)
{
    m_typeNum = typeNum;
}
