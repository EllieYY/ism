#ifndef LINEINTERCHANGEINFO_H
#define LINEINTERCHANGEINFO_H

#include "LineInfo.h"

class InterchangeInfo;
class LineInterchangeInfo : public LineInfo
{
    Q_OBJECT
public:
    explicit LineInterchangeInfo(QString name, QString picPath, QString color,
                                 QObject *parent = nullptr);

    QList<InterchangeInfo *> interchangeList() const;
    void setInterchangeList(const QList<InterchangeInfo *> &interchangeList);
    void addInterchangeInfo(InterchangeInfo *info);

private:
    QList<InterchangeInfo*> m_interchangeList;  // 换乘信息

signals:

};

#endif // LINEINTERCHANGEINFO_H
