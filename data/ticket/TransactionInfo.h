#ifndef TRANSACTIONINFO_H
#define TRANSACTIONINFO_H

#include <QObject>
#include <QDateTime>

/*
 * 交易信息
 */
class TransactionInfo : public QObject
{
    Q_OBJECT
public:
    explicit TransactionInfo(QString datetime, int type, long amount, QString samID,
                             QObject *parent = nullptr);

    QDateTime createTime() const;
    void setCreateTime(const QDateTime &createTime);

    int type() const;
    void setType(int type);

    long amount() const;
    void setAmount(long amount);

    QString samNum() const;
    void setSamNum(const QString &samNum);

private:
    QDateTime m_createTime;     // 交易时间
    int m_type;                 // 交易类型
    long m_amount;             // 交易金额(分)
//    int m_consumptionCount;     // 消费次数
//    float m_balance;            // 余额
//    int m_validCount;           // 剩余次数
    QString m_samNum;           // 交易终端Sam卡号

signals:

};

#endif // TRANSACTIONINFO_H
