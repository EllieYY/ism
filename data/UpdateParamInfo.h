#ifndef UPDATEPARAMINFO_H
#define UPDATEPARAMINFO_H

#include <QObject>

class UpdateParamInfo : public QObject
{
    Q_OBJECT
public:
    explicit UpdateParamInfo(QObject *parent = nullptr);

    long type() const;
    void setType(long type);

    long version() const;
    void setVersion(long version);

    QString fileName() const;
    void setFileName(const QString &fileName);

    bool fileOk() const;
    void setFileOk(bool fileOk);

    bool updated() const;
    void setUpdated(bool updated);

signals:

private:
    long m_type;
    long m_version;            // 当前版本
    QString m_fileName;        // 当前版本文件名
    bool m_fileOk;             // 文件下载完成
    bool m_updated;            // 是否更新成功
};

#endif // UPDATEPARAMINFO_H
