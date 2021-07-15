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
    explicit TransactionInfo(QDateTime createTime,
                             int type, float consumptionNum,
                             int consumptionCount, float balance,
                             int validCount, QObject *parent = nullptr);

    QDateTime createTime() const;
    void setCreateTime(const QDateTime &createTime);

    int type() const;
    void setType(int type);

    float consumptionNum() const;
    void setConsumptionNum(float consumptionNum);

    int consumptionCount() const;
    void setConsumptionCount(int consumptionCount);

    float balance() const;
    void setBalance(float balance);

    int validCount() const;
    void setValidCount(int validCount);

private:
    QDateTime m_createTime;     // 交易时间
    int m_type;                 // 交易类型
    float m_consumptionNum;     // 消费金额
    int m_consumptionCount;     // 消费次数
    float m_balance;            // 余额
    int m_validCount;           // 剩余次数

signals:

};

#endif // TRANSACTIONINFO_H
