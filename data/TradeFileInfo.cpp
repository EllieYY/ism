#include "TradeFileInfo.h"

TradeFileInfo::TradeFileInfo(QObject *parent) : QObject(parent)
{
    m_tradeSerial = 0;
    reset();

}

void TradeFileInfo::reset()
{
    setTradeSerial(m_tradeSerial + 1);
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

long TradeFileInfo::tradeSerial() const
{
    return m_tradeSerial;
}

void TradeFileInfo::setTradeSerial(long tradeSerial)
{
    m_tradeSerial = tradeSerial;
    if (m_tradeSerial > 999999) {
        m_tradeSerial = 1;
    }
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

void TradeFileInfo::setFileNameSet(const QSet<QString> &fileNameSet)
{
    m_fileNameSet = fileNameSet;
}
