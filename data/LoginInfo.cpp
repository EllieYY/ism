#include "LoginInfo.h"

LoginInfo::LoginInfo(QObject *parent) : QObject(parent)
{

}

QString LoginInfo::userName() const
{
    return m_userName;
}

void LoginInfo::setUserName(const QString &userName)
{
    m_userName = userName;
}

QString LoginInfo::password() const
{
    return m_password;
}

void LoginInfo::setPassword(const QString &password)
{
    m_password = password;
}

QDateTime LoginInfo::loginTime() const
{
    return m_loginTime;
}

void LoginInfo::setLoginTime(const QDateTime &loginTime)
{
    m_loginTime = loginTime;
}

QDateTime LoginInfo::logoutTime() const
{
    return m_logoutTime;
}

void LoginInfo::setLogoutTime(const QDateTime &logoutTime)
{
    m_logoutTime = logoutTime;
}
