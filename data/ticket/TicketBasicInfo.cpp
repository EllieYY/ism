#include "TicketBasicInfo.h"

TicketBasicInfo::TicketBasicInfo(int type, QString typeStr, QString number, QString createTime, QString validDate,
                       int cardState, int tripState, long balance, QObject *parent) : QObject(parent)
{
    m_typeNum = type;
    m_type = typeStr;
    m_number = number;
    m_createTime = QDate::fromString(createTime, "yyyyMMdd");
    m_validDate = QDate::fromString(validDate, "yyyyMMdd");
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

bool TicketBasicInfo::isAllowUpdate() const
{
    return m_isAllowUpdate;
}

void TicketBasicInfo::setIsAllowUpdate(bool isAllowUpdate)
{
    m_isAllowUpdate = isAllowUpdate;
}

bool TicketBasicInfo::isAllowOctPay() const
{
    return m_isAllowOctPay;
}

void TicketBasicInfo::setIsAllowOctPay(bool isAllowOctPay)
{
    m_isAllowOctPay = isAllowOctPay;
}

int TicketBasicInfo::updateType() const
{
    return m_updateType;
}

void TicketBasicInfo::setUpdateType(int updateType)
{
    m_updateType = updateType;
}


QDateTime TicketBasicInfo::enTime() const
{
    return m_enTime;
}

void TicketBasicInfo::setEnTime(const QString &enTime)
{
    m_enTime = QDateTime::fromString(enTime, "yyyyMMddHHmmss");
}

QDateTime TicketBasicInfo::exTime() const
{
    return m_exTime;
}

void TicketBasicInfo::setExTime(const QString &exTime)
{
    m_exTime = QDateTime::fromString(exTime, "yyyyMMddHHmmss");
}

QString TicketBasicInfo::enStationCode() const
{
    return m_enStationCode;
}

void TicketBasicInfo::setEnStationCode(const QString &enStationCode)
{
    m_enStationCode = enStationCode;
}

QString TicketBasicInfo::exStationCode() const
{
    return m_exStationCode;
}

void TicketBasicInfo::setExStationCode(const QString &exStationCode)
{
    m_exStationCode = exStationCode;
}

uint TicketBasicInfo::updateAmount() const
{
    return m_updateAmount;
}

void TicketBasicInfo::setUpdateAmount(const uint &updateAmount)
{
    m_updateAmount = updateAmount;
}

int TicketBasicInfo::icType() const
{
    return m_icType;
}

void TicketBasicInfo::setIcType(int icType)
{
    m_icType = icType;
}

int TicketBasicInfo::errorCode() const
{
    return m_errorCode;
}

void TicketBasicInfo::setErrorCode(int errorCode)
{
    m_errorCode = errorCode;
}
