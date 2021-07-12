#ifndef LINEINFO_H
#define LINEINFO_H

#include <QObject>

class LineInfo : public QObject
{
    Q_OBJECT
public:
    explicit LineInfo(QString name, QString picPath,
                      QString color, QObject *parent = nullptr);

public:
    QString getName() const;
    void setName(const QString &name);

    QString getPicPath() const;
    void setPicPath(const QString &picPath);

    QString getColor() const;
    void setColor(const QString &color);

private:
    QString m_name;                     // 线路名称
    QString m_picPath;                  // 线路图（相对）路径
    QString m_color;                    // 线路颜色值

signals:

};

#endif // LINEINFO_H
