#include "TransactionInfo.h"
#include <QDateTime>

TransactionInfo::TransactionInfo(QString datetime, int type, long amount, QString samID,
                                 QObject *parent) : QObject(parent)
{    
    m_createTime = QDateTime::fromString(datetime, "yyyyMMddHHmmss");
    m_type = type;
    m_amount = amount;
    m_samNum = samID;
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

long TransactionInfo::amount() const
{
    return m_amount;
}

void TransactionInfo::setAmount(long amount)
{
    m_amount = amount;
}

QString TransactionInfo::samNum() const
{
    return m_samNum;
}

void TransactionInfo::setSamNum(const QString &samNum)
{
    m_samNum = samNum;
}

