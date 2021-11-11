#ifndef BASICINFO_H
#define BASICINFO_H

#include <QObject>

class BasicInfo : public QObject
{
    Q_OBJECT
public:
    explicit BasicInfo(QObject *parent = nullptr);

    QString stationName() const;
    void setStationName(const QString &stationName);

    QString deviceId() const;
    void setDeviceId(const QString &deviceId);

    QString ismServiceIp() const;
    void setIsmServiceIp(const QString &ismServiceIp);

    long ismServicePort() const;
    void setIsmServicePort(long ismServicePort);

    QString asrServiceIp() const;
    void setAsrServiceIp(const QString &asrServiceIp);

    long asrServicePort() const;
    void setAsrServicePort(long asrServicePort);

    QString appkey() const;
    void setAppkey(const QString &appkey);

    QString secret() const;
    void setSecret(const QString &secret);

    QString stationCode() const;
    void setStationCode(const QString &stationCode);

    bool isPayZone() const;
    void setIsPayZone(bool isPayZone);

    QString scId() const;
    void setScId(const QString &scId);

    QString scIP() const;
    void setScIP(const QString &scIP);

    long scPort() const;
    void setScPort(long scPort);

    QString localIP() const;
    void setLocalIP(const QString &localIP);

    long localPort() const;
    void setLocalPort(long localPort);

    int antiNo() const;
    void setAntiNo(int antiNo);

    int readerPort() const;
    void setReaderPort(int readerPort);

    int bimPort() const;
    void setBimPort(int bimPort);

    int f53Port() const;
    void setF53Port(int f53Port);

    int brcPort() const;
    void setBrcPort(int brcPort);

private:
    QString     m_stationName;      // 站点名称
    QString     m_stationCode;      // 站点编号
    QString     m_deviceId;         // 设备编号
    bool        m_isPayZone;        // 是否是付费区

    /* 智能客服后台信息 */
    QString     m_ismServiceIp;
    long        m_ismServicePort;

    /* asr服务信息 */
    QString     m_asrServiceIp;
    long        m_asrServicePort;
    QString     m_appkey;
    QString     m_secret;

    /* AFC相关信息 */
    QString     m_scId;    // sc编号
    QString     m_scIP;
    long        m_scPort;
    QString     m_localIP;   // 默认 0.0.0.0
    long        m_localPort;

    /* 硬件设备信息 */
    int         m_antiNo;           // 读写器天线
    int         m_readerPort;
    int         m_bimPort;          // 纸币模块
    int         m_f53Port;          // 找零模块
    int         m_brcPort;          // 硬币模块

signals:

};

#endif // BASICINFO_H
