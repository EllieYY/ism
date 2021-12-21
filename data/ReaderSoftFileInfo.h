#ifndef READERSOFTFILEINFO_H
#define READERSOFTFILEINFO_H

#include <QObject>
/*
 * 读写器程序更新情况记录
*/
class ReaderSoftFileInfo : public QObject
{
    Q_OBJECT
public:
    explicit ReaderSoftFileInfo(QObject *parent = nullptr);

    QString getFileName() const;
    void setFileName(const QString &value);

    QString getDateString();

    bool getHasUpdated() const;
    void setHasUpdated(bool value);

    bool getFileReady() const;
    void setFileReady(bool value);

private:
    QString fileName;   // 文件名
    QString dateString; // 更新日期
    bool hasUpdated;    // 程序更新情况
    bool fileReady;     // 程序文件下载情况

signals:

};

#endif // READERSOFTFILEINFO_H
