#include "TradeFileInfo.h"

TradeFileInfo::TradeFileInfo(QObject *parent) : QObject(parent)
{
    m_fileTradeSerial = 0;
    m_deviceTradeSerial = 1;
    reset();
}

void TradeFileInfo::reset()
{
    setFileTradeSerial(m_fileTradeSerial + 1);
    m_fileCount = 0;
    m_datetime = QDateTime::currentDateTime();
    m_fileNameSet.clear();
}

int TradeFileInfo::fileCount() const
{
    return m_fileCount;
}

void TradeFileInfo::setFileCount(int fileCount)
{
    m_fileCount = fileCount;
}

QSet<QString> TradeFileInfo::fileNameSet() const
{
    return m_fileNameSet;
}

void TradeFileInfo::addFileName(QString name)
{
    m_fileCount++;
    m_fileNameSet.insert(name);
}

QDateTime TradeFileInfo::datetime() const
{
    return m_datetime;
}

void TradeFileInfo::setDatetime(const QDateTime &datetime)
{
    m_datetime = datetime;
}

ulong TradeFileInfo::deviceTradeSerial() const
{
    return m_deviceTradeSerial;
}

void TradeFileInfo::setDeviceTradeSerial(const ulong &deviceTradeSerial)
{
    m_deviceTradeSerial = deviceTradeSerial;
}

ulong TradeFileInfo::fileTradeSerial() const
{

    return m_fileTradeSerial;
}

void TradeFileInfo::setFileTradeSerial(const ulong &fileTradeSerial)
{
    m_fileTradeSerial = fileTradeSerial;
    if (m_fileTradeSerial > 999999) {
        m_fileTradeSerial = 1;
    }
}


void TradeFileInfo::setFileNameSet(const QSet<QString> &fileNameSet)
{
    m_fileNameSet = fileNameSet;
}
