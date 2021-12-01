#include "X7000FileInfo.h"

X7000FileInfo::X7000FileInfo(QObject *parent) : QObject(parent)
{

}

QString X7000FileInfo::fileName() const
{
    return m_fileName;
}

void X7000FileInfo::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

int X7000FileInfo::type() const
{
    return m_type;
}

void X7000FileInfo::setType(int type)
{
    m_type = type;
}

QByteArray X7000FileInfo::md5Arr() const
{
    return m_md5Arr;
}

void X7000FileInfo::setMd5Arr(const QByteArray &md5Arr)
{
    m_md5Arr = md5Arr;
}
