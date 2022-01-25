#include "TradeFileInfo.h"
#include <QDebug>

TradeFileInfo::TradeFileInfo(QObject *parent) : QObject(parent)
{
    m_fileTradeSerial = 1;
    m_deviceTradeSerial = 1;
    reset();
}

void TradeFileInfo::reset()
{
//    qDebug() << "reset ---------";
//    setFileTradeSerial(m_fileTradeSerial + 1);
    m_datetime = QDateTime::currentDateTime();
    m_fileNameSet.clear();
    m_fileCount = m_fileNameSet.size();
    m_typeSerialMap.clear();
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
//    m_fileCount++;
    m_fileNameSet.insert(name);

//    qDebug() << "[addFileName] m_fileCount=" << m_fileCount << ", m_fileNameSet.size()=" << m_fileNameSet.size();
//    // 不同类别的交易文件增加，也会导致流水号的变化
//    if (m_fileCount != m_fileNameSet.size()) {
//        setFileTradeSerial(m_fileTradeSerial + 1);
//    }
    m_fileCount = m_fileNameSet.size();
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

ulong TradeFileInfo::fileTradeSerialByType(int icType)
{
    if (!m_typeSerialMap.contains(icType)) {
        m_typeSerialMap.insert(icType, m_fileTradeSerial);
        setFileTradeSerial(m_fileTradeSerial + 1);
    }

    qDebug() << "type:" << icType << ", serial:" << m_typeSerialMap.value(icType);
    return m_typeSerialMap.value(icType);
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
    qDebug() << "[setFileTradeSerial] " << m_fileTradeSerial;
}


void TradeFileInfo::setFileNameSet(const QSet<QString> &fileNameSet)
{
    m_fileNameSet = fileNameSet;
    m_fileCount = m_fileNameSet.size();
}
