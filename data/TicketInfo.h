#ifndef TICKETINFO_H
#define TICKETINFO_H

#include <QObject>
#include <QDate>

class TicketInfo : public QObject
{
    Q_OBJECT
public:
    explicit TicketInfo(QString type, QString number, QDate createTime, QDate validDate,
    int cardState, int tripState, float balance, QObject *parent = nullptr);

    QString type() const;
    void setType(const QString &type);

    QString number() const;
    void setNumber(const QString &number);

    QDate createTime() const;
    void setCreateTime(const QDate &createTime);

    QDate validDate() const;
    void setValidDate(const QDate &validDate);

    int cardState() const;
    void setCardState(int cardState);

    int tripState() const;
    void setTripState(int tripState);

    float balance() const;
    void setBalance(float balance);

private:
    QString     m_type;         // 票种
    QString     m_number;       // 卡号
    QDate       m_createTime;   // 发卡时间
    QDate       m_validDate;    // 有效日期
    int         m_cardState;    // 卡状态
    int         m_tripState;    // 旅程状态
    float       m_balance;      // 余额


signals:

};

#endif // TICKETINFO_H
