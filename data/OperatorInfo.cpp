#include "OperatorInfo.h"

OperatorInfo::OperatorInfo(QObject *parent) : QObject(parent)
{

}

QString OperatorInfo::code() const
{
    return m_code;
}

void OperatorInfo::setCode(const QString &code)
{
    m_code = code;
}

QString OperatorInfo::name() const
{
    return m_name;
}

void OperatorInfo::setName(const QString &name)
{
    m_name = name;
}

QString OperatorInfo::card() const
{
    return m_card;
}

void OperatorInfo::setCard(const QString &card)
{
    m_card = card;
}

QString OperatorInfo::pwd() const
{
    return m_pwd;
}

void OperatorInfo::setPwd(const QString &pwd)
{
    m_pwd = pwd;
}

int OperatorInfo::type() const
{
    return m_type;
}

void OperatorInfo::setType(int type)
{
    m_type = type;
}

int OperatorInfo::access() const
{
    return m_access;
}

void OperatorInfo::setAccess(int access)
{
    m_access = access;
}

QDateTime OperatorInfo::validDate() const
{
    return m_validDate;
}

void OperatorInfo::setValidDate(QString dateStr)
{
    m_validDate = QDateTime::fromString(dateStr, "yyyyMMddHHmmss");
}
