#ifndef STATION_H
#define STATION_H

#include <QObject>

/*
 * 站点信息
 */
class Station : public QObject
{
    Q_OBJECT
public:
    explicit Station(QString name, QString code, QObject *parent = nullptr);

    QString name() const;
    void setName(const QString &name);

    QString code() const;
    void setCode(const QString &code);

private:
    QString m_name;
    QString m_code;

signals:

};

#endif // STATION_H
