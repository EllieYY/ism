#ifndef CARDINFO_H
#define CARDINFO_H

#include <QObject>
#include <QDateTime>

class CardInfo : public QObject
{
    Q_OBJECT
public:
    explicit CardInfo(int type, int m_state,
                      QString m_enterStation, QDateTime m_enterTime,
                      QString m_exitStation, QDateTime m_exitTime,
                      QDate m_validDate, float m_arrearage, QString m_reason,
                      QObject *parent = nullptr);

    int type() const;
    void setType(int type);

    int state() const;
    void setState(int state);

    QString enterStation() const;
    void setEnterStation(const QString &enterStation);

    QDateTime enterTime() const;
    void setEnterTime(const QDateTime &enterTime);

    QString exitStation() const;
    void setExitStation(const QString &exitStation);

    QDateTime exitTime() const;
    void setExitTime(const QDateTime &exitTime);

    QDate validDate() const;
    void setValidDate(const QDate &validDate);

    float arrearage() const;
    void setArrearage(float arrearage);

    QString reason() const;
    void setReason(const QString &reason);

private:
    int m_type;                 // 类型
    int m_state;                // 车票状态
    QString m_enterStation;     // 进站车站
    QDateTime m_enterTime;      // 进站时间
    QString m_exitStation;      // 出站车站
    QDateTime m_exitTime;       // 出站时间
    QDate m_validDate;          // 有效期限
    float m_arrearage;          // 补票金额
    QString m_reason;           // 补票原因

signals:

};

#endif // CARDINFO_H
