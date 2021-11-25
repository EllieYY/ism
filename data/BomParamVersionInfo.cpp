#include "BomParamVersionInfo.h"

BomParamVersionInfo::BomParamVersionInfo(long type, long version, QString fileName, QObject *parent) : QObject(parent)
{
    m_type = type;
    m_version = version;
    m_fileName = fileName;
}

long BomParamVersionInfo::type() const
{
    return m_type;
}

void BomParamVersionInfo::setType(long type)
{
    m_type = type;
}

long BomParamVersionInfo::version() const
{
    return m_version;
}

void BomParamVersionInfo::setVersion(long version)
{
    m_version = version;
}

QString BomParamVersionInfo::fileName() const
{
    return m_fileName;
}

void BomParamVersionInfo::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}
