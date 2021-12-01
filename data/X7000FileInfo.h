#ifndef X7000FILEINFO_H
#define X7000FILEINFO_H

#include <QObject>

class X7000FileInfo : public QObject
{
    Q_OBJECT
public:
    explicit X7000FileInfo(QObject *parent = nullptr);

    QString fileName() const;
    void setFileName(const QString &fileName);

    int type() const;
    void setType(int type);

    QByteArray md5Arr() const;
    void setMd5Arr(const QByteArray &md5Arr);

private:
    QString m_fileName;
    QByteArray m_md5Arr;
    int m_type;

signals:

};

#endif // X7000FILEINFO_H
