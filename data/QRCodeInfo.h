#ifndef QRCODEINFO_H
#define QRCODEINFO_H

#include <QObject>
#include <QDateTime>

class QRCodeInfo : public QObject
{
    Q_OBJECT
public:
    explicit QRCodeInfo(QString tips, int channel,
                        QString enterStation, QDateTime enterTime,
                        QString exitStation, bool canReregister,
                        QObject *parent = nullptr);

    QString tips() const;
    void setTips(const QString &tips);

    int channel() const;
    void setChannel(int channel);

    QString enterStation() const;
    void setEnterStation(const QString &enterStation);

    QDateTime enterTime() const;
    void setEnterTime(const QDateTime &enterTime);

    QString exitStation() const;
    void setExitStation(const QString &exitStation);

    bool canReregister() const;
    void setCanReregister(bool canReregister);

private:
    QString m_tips;
    int m_channel;
    QString m_enterStation;
    QDateTime m_enterTime;
    QString m_exitStation;
    bool m_canReregister;
signals:

};

#endif // QRCODEINFO_H
