#include "BomParamVersionInfo.h"

BomParamVersionInfo::BomParamVersionInfo(QObject *parent) : QObject(parent)
{
    m_version = 0;
    m_preVersion = 0;         // 上一个版本
    m_preFileName = "";       // 上一个版本文件名
    m_readerUsed = true;      // 是否是读写器使用的参数
    m_readerUpdated = true;   // 默认读写器已经更新了
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

long BomParamVersionInfo::preVersion() const
{
    return m_preVersion;
}

void BomParamVersionInfo::setPreVersion(long preVersion)
{
    m_preVersion = preVersion;
}

QString BomParamVersionInfo::preFileName() const
{
    return m_preFileName;
}

void BomParamVersionInfo::setPreFileName(const QString &preFileName)
{
    m_preFileName = preFileName;
}

bool BomParamVersionInfo::readerUsed() const
{
    return m_readerUsed;
}

void BomParamVersionInfo::setReaderUsed(bool readerUsed)
{
    m_readerUsed = readerUsed;
}

bool BomParamVersionInfo::readerUpdated() const
{
    return m_readerUpdated;
}

void BomParamVersionInfo::setReaderUpdated(bool readerUpdated)
{
    m_readerUpdated = readerUpdated;
}
