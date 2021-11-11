#ifndef TICKETBASICINFO_H
#define TICKETBASICINFO_H

#include <QObject>
#include <QDate>

/*
 * 票卡基本信息
 */
class TicketBasicInfo : public QObject
{
    Q_OBJECT
public:
    explicit TicketBasicInfo(int type, QString typeStr, QString number, QString createTime, QString validDate,
    int cardState, int tripState, long balance, QObject *parent = nullptr);

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

    int typeNum() const;
    void setTypeNum(int typeNum);

private:
    QString     m_type;         // 卡类型
    QString     m_number;       // 卡号
    QDate       m_createTime;   // 发卡时间
    QDate       m_validDate;    // 有效期
    int         m_cardState;    // 卡状态
    int         m_tripState;    // 旅程状态
    float       m_balance;      // 余额
    int         m_typeNum;


signals:

};

#endif // TICKETBASICINFO_H
