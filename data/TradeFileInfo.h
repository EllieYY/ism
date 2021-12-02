#ifndef TRADEFILEINFO_H
#define TRADEFILEINFO_H

#include <QObject>
#include <QSet>
#include <QDateTime>

class TradeFileInfo : public QObject
{
    Q_OBJECT
public:
    explicit TradeFileInfo(QObject *parent = nullptr);

    void reset();

    int fileCount() const;
    void setFileCount(int fileCount);

    QSet<QString> fileNameSet() const;
    void addFileName(QString name);
    void setFileNameSet(const QSet<QString> &fileNameSet);

    QDateTime datetime() const;
    void setDatetime(const QDateTime &datetime);

    ulong deviceTradeSerial() const;
    void setDeviceTradeSerial(const ulong &deviceTradeSerial);

    ulong fileTradeSerial() const;
    void setFileTradeSerial(const ulong &fileTradeSerial);

private:
    int m_fileCount;            // 文件个数
    ulong m_fileTradeSerial;         // 交易文件序列号
    QDateTime m_datetime;
    QSet<QString> m_fileNameSet;  // 交易文件名称列表
    ulong m_deviceTradeSerial;    // 终端交易流水号   从1开始，不重复

signals:

};

#endif // TRADEFILEINFO_H
