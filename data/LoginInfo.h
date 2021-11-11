#ifndef LOGININFO_H
#define LOGININFO_H

#include <QObject>
#include <QDateTime>

class LoginInfo : public QObject
{
    Q_OBJECT
public:
    explicit LoginInfo(QObject *parent = nullptr);

    QString userName() const;
    void setUserName(const QString &userName);

    QString password() const;
    void setPassword(const QString &password);

    QDateTime loginTime() const;
    void setLoginTime(const QDateTime &loginTime);

    QDateTime logoutTime() const;
    void setLogoutTime(const QDateTime &logoutTime);

signals:

private:
    QString     m_userName;
    QString     m_password;
    QDateTime   m_loginTime;
    QDateTime   m_logoutTime;

};

#endif // LOGININFO_H
