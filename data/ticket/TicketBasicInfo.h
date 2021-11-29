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

    bool isAllowUpdate() const;
    void setIsAllowUpdate(bool isAllowUpdate);

    bool isAllowOctPay() const;
    void setIsAllowOctPay(bool isAllowOctPay);

    int updateType() const;
    void setUpdateType(int updateType);

    QDateTime enTime() const;
    void setEnTime(const QString &enTime);

    QDateTime exTime() const;
    void setExTime(const QString &exTime);

    QString enStationCode() const;
    void setEnStationCode(const QString &enStationCode);

    QString exStationCode() const;
    void setExStationCode(const QString &exStationCode);

    uint updateAmount() const;
    void setUpdateAmount(const uint &updateAmount);

    int icType() const;
    void setIcType(int icType);

    int errorCode() const;
    void setErrorCode(int errorCode);

private:
    QString     m_type;         // 卡类型
    QString     m_number;       // 卡号
    QDate       m_createTime;   // 发卡时间
    QDate       m_validDate;    // 有效期
    int         m_cardState;    // 卡状态
    int         m_tripState;    // 旅程状态
    float       m_balance;      // 余额
    int         m_typeNum;      // ticketType
    int         m_icType;

    bool        m_isAllowUpdate;  // 是否允许更新
    bool        m_isAllowOctPay;  // 是否允许卡扣更新
    int         m_updateType;     // 更新类型
    QString     m_enStationCode;  // 进站车站编号
    QString     m_exStationCode;  // 出站车站编号
    QDateTime   m_enTime;         // 进站时间
    QDateTime   m_exTime;         // 出站时间
    uint        m_updateAmount;   // 票卡更新应收罚金

    int   m_errorCode;    // 票卡分析错误码

signals:

};

#endif // TICKETBASICINFO_H
