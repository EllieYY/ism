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

    long tradeSerial() const;
    void setTradeSerial(long tradeSerial);

    QSet<QString> fileNameSet() const;
    void addFileName(QString name);
    void setFileNameSet(const QSet<QString> &fileNameSet);

    QDateTime datetime() const;
    void setDatetime(const QDateTime &datetime);

private:
    int m_fileCount;            // 文件个数
    long m_tradeSerial;         // 交易文件序列号
    QDateTime m_datetime;
    QSet<QString> m_fileNameSet;  // 交易文件名称列表

signals:

};

#endif // TRADEFILEINFO_H
