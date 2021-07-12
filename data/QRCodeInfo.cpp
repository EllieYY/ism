#include "QRCodeInfo.h"

QRCodeInfo::QRCodeInfo(QString tips, int channel,
                       QString enterStation, QDateTime enterTime,
                       QString exitStation, bool canReregister,
                       QObject *parent) : QObject(parent)
{
    m_tips = tips;
    m_channel = channel;
    m_enterStation = enterStation;
    m_enterTime = enterTime;
    m_exitStation = exitStation;
    m_canReregister = canReregister;
}

QString QRCodeInfo::tips() const
{
    return m_tips;
}

void QRCodeInfo::setTips(const QString &tips)
{
    m_tips = tips;
}

int QRCodeInfo::channel() const
{
    return m_channel;
}

void QRCodeInfo::setChannel(int channel)
{
    m_channel = channel;
}

QString QRCodeInfo::enterStation() const
{
    return m_enterStation;
}

void QRCodeInfo::setEnterStation(const QString &enterStation)
{
    m_enterStation = enterStation;
}

QDateTime QRCodeInfo::enterTime() const
{
    return m_enterTime;
}

void QRCodeInfo::setEnterTime(const QDateTime &enterTime)
{
    m_enterTime = enterTime;
}

QString QRCodeInfo::exitStation() const
{
    return m_exitStation;
}

void QRCodeInfo::setExitStation(const QString &exitStation)
{
    m_exitStation = exitStation;
}

bool QRCodeInfo::canReregister() const
{
    return m_canReregister;
}

void QRCodeInfo::setCanReregister(bool canReregister)
{
    m_canReregister = canReregister;
}
