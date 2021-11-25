#ifndef BOMPARAMVERSIONINFO_H
#define BOMPARAMVERSIONINFO_H

#include <QObject>

class BomParamVersionInfo : public QObject
{
    Q_OBJECT
public:
    explicit BomParamVersionInfo(long type, long version, QString fileName, QObject *parent = nullptr);

    long type() const;
    void setType(long type);

    long version() const;
    void setVersion(long version);

    QString fileName() const;
    void setFileName(const QString &fileName);

private:
    long m_type;
    long m_version;
    QString m_fileName;

signals:

};

#endif // BOMPARAMVERSIONINFO_H
