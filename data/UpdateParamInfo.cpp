#include "UpdateParamInfo.h"

UpdateParamInfo::UpdateParamInfo(QObject *parent) : QObject(parent)
{
    m_fileOk = false;             // 文件下载完成
    m_updated = false;            // 是否更新成功
}

long UpdateParamInfo::type() const
{
    return m_type;
}

void UpdateParamInfo::setType(long type)
{
    m_type = type;
}

long UpdateParamInfo::version() const
{
    return m_version;
}

void UpdateParamInfo::setVersion(long version)
{
    m_version = version;
}

QString UpdateParamInfo::fileName() const
{
    return m_fileName;
}

void UpdateParamInfo::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

bool UpdateParamInfo::fileOk() const
{
    return m_fileOk;
}

void UpdateParamInfo::setFileOk(bool fileOk)
{
    m_fileOk = fileOk;
}

bool UpdateParamInfo::updated() const
{
    return m_updated;
}

void UpdateParamInfo::setUpdated(bool updated)
{
    m_updated = updated;
}
