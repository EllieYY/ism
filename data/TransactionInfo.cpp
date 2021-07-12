#include "TransactionInfo.h"

TransactionInfo::TransactionInfo(QDateTime createTime, int type,
                                 float consumptionNum, int consumptionCount,
                                 float balance, int validCount, QObject *parent) : QObject(parent)
{
    m_createTime = createTime;
    m_type = type;
    m_consumptionNum = consumptionNum;
    m_consumptionCount = consumptionCount;
    m_balance = balance;
    m_validCount = validCount;
}

QDateTime TransactionInfo::createTime() const
{
    return m_createTime;
}

void TransactionInfo::setCreateTime(const QDateTime &createTime)
{
    m_createTime = createTime;
}

int TransactionInfo::type() const
{
    return m_type;
}

void TransactionInfo::setType(int type)
{
    m_type = type;
}

float TransactionInfo::consumptionNum() const
{
    return m_consumptionNum;
}

void TransactionInfo::setConsumptionNum(float consumptionNum)
{
    m_consumptionNum = consumptionNum;
}

int TransactionInfo::consumptionCount() const
{
    return m_consumptionCount;
}

void TransactionInfo::setConsumptionCount(int consumptionCount)
{
    m_consumptionCount = consumptionCount;
}

float TransactionInfo::balance() const
{
    return m_balance;
}

void TransactionInfo::setBalance(float balance)
{
    m_balance = balance;
}

int TransactionInfo::validCount() const
{
    return m_validCount;
}

void TransactionInfo::setValidCount(int validCount)
{
    m_validCount = validCount;
}
