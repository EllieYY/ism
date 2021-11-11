#ifndef REREGISTERINFO_H
#define REREGISTERINFO_H

#include <QObject>
#include <QDateTime>

class ReregisterInfo : public QObject
{
    Q_OBJECT
public:
    explicit ReregisterInfo(QObject *parent = nullptr);

    int type() const;
    void setType(int type);

    int state() const;
    void setState(int state);

    QDate validDate() const;
    void setValidDate(const QDate &validDate);

    QString enterStation() const;
    void setEnterStation(const QString &enterStation);

    QDateTime enterTime() const;
    void setEnterTime(const QDateTime &enterTime);

    QString exitStation() const;
    void setExitStation(const QString &exitStation);

    QDateTime exitTime() const;
    void setExitTime(const QDateTime &exitTime);

    QString reason() const;
    void setReason(const QString &reason);

    QString number() const;
    void setNumber(const QString &number);

    QDate createTime() const;
    void setCreateTime(const QDate &createTime);

    float balance() const;
    void setBalance(float balance);

    int reregisterType() const;
    void setReregisterType(int reregisterType);

private:
    int         m_type;                 // 卡类型
    QString     m_number;           // 卡号
    int         m_state;                // 卡状态
    QDate       m_createTime;   // 发卡时间
    QDate       m_validDate;    // 有效期
    float       m_balance;      // 余额

    QString     m_enterStation;     // 进站车站
    QDateTime   m_enterTime;      // 进站时间
    QString     m_exitStation;      // 出站车站
    QDateTime   m_exitTime;       // 出站时间
    QString     m_reason;           // 补票原因
    int         m_reregisterType;      // 票卡更新类型: REREGISTER_TYPE


signals:

};

#endif // REREGISTERINFO_H
