#ifndef ASRERROR_H
#define ASRERROR_H

#include <QObject>

class AsrError : public QObject
{
    Q_OBJECT
public:
    explicit AsrError(QObject *parent = nullptr);


    int code() const;
    void setCode(int code);

    QString message() const;
    void setMessage(const QString &message);

private:
    int m_code;          // 错误码
    QString m_message;   // 错误信息

signals:

};

#endif // ASRERROR_H
