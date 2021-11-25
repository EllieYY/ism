#ifndef OPERATORINFO_H
#define OPERATORINFO_H

#include <QObject>
#include <QDateTime>

class OperatorInfo : public QObject
{
    Q_OBJECT
public:
    explicit OperatorInfo(QObject *parent = nullptr);

    QString code() const;
    void setCode(const QString &code);

    QString name() const;
    void setName(const QString &name);

    QString card() const;
    void setCard(const QString &card);

    QString pwd() const;
    void setPwd(const QString &pwd);

    int type() const;
    void setType(int type);

    int access() const;
    void setAccess(int access);

    QDateTime validDate() const;
    void setValidDate(QString dateStr);

private:
    QString m_code;
    QString m_name;
    QString m_card;
    QString m_pwd;
    int m_type;
    int m_access;
    QDateTime m_validDate;


signals:

};

#endif // OPERATORINFO_H
