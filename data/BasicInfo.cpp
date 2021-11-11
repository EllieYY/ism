#include "BasicInfo.h"

BasicInfo::BasicInfo(QObject *parent) : QObject(parent)
{

}

QString BasicInfo::stationName() const
{
    return m_stationName;
}

void BasicInfo::setStationName(const QString &stationName)
{
    m_stationName = stationName;
}

QString BasicInfo::deviceId() const
{
    return m_deviceId;
}

void BasicInfo::setDeviceId(const QString &deviceId)
{
    m_deviceId = deviceId;
}

QString BasicInfo::ismServiceIp() const
{
    return m_ismServiceIp;
}

void BasicInfo::setIsmServiceIp(const QString &ismServiceIp)
{
    m_ismServiceIp = ismServiceIp;
}

long BasicInfo::ismServicePort() const
{
    return m_ismServicePort;
}

void BasicInfo::setIsmServicePort(long ismServicePort)
{
    m_ismServicePort = ismServicePort;
}

QString BasicInfo::asrServiceIp() const
{
    return m_asrServiceIp;
}

void BasicInfo::setAsrServiceIp(const QString &asrServiceIp)
{
    m_asrServiceIp = asrServiceIp;
}

long BasicInfo::asrServicePort() const
{
    return m_asrServicePort;
}

void BasicInfo::setAsrServicePort(long asrServicePort)
{
    m_asrServicePort = asrServicePort;
}

QString BasicInfo::appkey() const
{
    return m_appkey;
}

void BasicInfo::setAppkey(const QString &appkey)
{
    m_appkey = appkey;
}

QString BasicInfo::secret() const
{
    return m_secret;
}

void BasicInfo::setSecret(const QString &secret)
{
    m_secret = secret;
}

QString BasicInfo::stationCode() const
{
    return m_stationCode;
}

void BasicInfo::setStationCode(const QString &stationCode)
{
    m_stationCode = stationCode;
}

bool BasicInfo::isPayZone() const
{
    return m_isPayZone;
}

void BasicInfo::setIsPayZone(bool isPayZone)
{
    m_isPayZone = isPayZone;
}

QString BasicInfo::scId() const
{
    return m_scId;
}

void BasicInfo::setScId(const QString &scId)
{
    m_scId = scId;
}

QString BasicInfo::scIP() const
{
    return m_scIP;
}

void BasicInfo::setScIP(const QString &scIP)
{
    m_scIP = scIP;
}

long BasicInfo::scPort() const
{
    return m_scPort;
}

void BasicInfo::setScPort(long scPort)
{
    m_scPort = scPort;
}

QString BasicInfo::localIP() const
{
    return m_localIP;
}

void BasicInfo::setLocalIP(const QString &localIP)
{
    m_localIP = localIP;
}

long BasicInfo::localPort() const
{
    return m_localPort;
}

void BasicInfo::setLocalPort(long localPort)
{
    m_localPort = localPort;
}

int BasicInfo::antiNo() const
{
    return m_antiNo;
}

void BasicInfo::setAntiNo(int antiNo)
{
    m_antiNo = antiNo;
}

int BasicInfo::readerPort() const
{
    return m_readerPort;
}

void BasicInfo::setReaderPort(int readerPort)
{
    m_readerPort = readerPort;
}

int BasicInfo::bimPort() const
{
    return m_bimPort;
}

void BasicInfo::setBimPort(int bimPort)
{
    m_bimPort = bimPort;
}

int BasicInfo::f53Port() const
{
    return m_f53Port;
}

void BasicInfo::setF53Port(int f53Port)
{
    m_f53Port = f53Port;
}

int BasicInfo::brcPort() const
{
    return m_brcPort;
}

void BasicInfo::setBrcPort(int brcPort)
{
    m_brcPort = brcPort;
}
