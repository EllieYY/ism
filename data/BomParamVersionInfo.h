#ifndef BOMPARAMVERSIONINFO_H
#define BOMPARAMVERSIONINFO_H

#include <QObject>

class BomParamVersionInfo : public QObject
{
    Q_OBJECT
public:
    explicit BomParamVersionInfo(QObject *parent = nullptr);

    long type() const;
    void setType(long type);

    long version() const;
    void setVersion(long version);

    QString fileName() const;
    void setFileName(const QString &fileName);

    long preVersion() const;
    void setPreVersion(long preVersion);

    QString preFileName() const;
    void setPreFileName(const QString &preFileName);

    bool readerUsed() const;
    void setReaderUsed(bool readerUsed);

    bool readerUpdated() const;
    void setReaderUpdated(bool readerUpdated);

private:
    long m_type;
    long m_version;            // 当前版本
    long m_preVersion;         // 上一个版本
    QString m_fileName;        // 当前版本文件名
    QString m_preFileName;     // 上一个版本文件名
    bool m_readerUsed;         // 是否是读写器使用的参数
    bool m_readerUpdated;      // 读写器是否已更新

signals:

};

#endif // BOMPARAMVERSIONINFO_H
