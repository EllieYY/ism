#include "LineInterchangeInfo.h"
#include "InterchangeInfo.h"

LineInterchangeInfo::LineInterchangeInfo(QString name, QString picPath,
                                         QString color, QObject *parent) :
                  LineInfo(name, picPath,color,parent)
{

}

QList<InterchangeInfo *> LineInterchangeInfo::interchangeList() const
{
    return m_interchangeList;
}

void LineInterchangeInfo::setInterchangeList(const QList<InterchangeInfo *> &interchangeList)
{
    m_interchangeList = interchangeList;
}

void LineInterchangeInfo::addInterchangeInfo(InterchangeInfo *info)
{
    m_interchangeList.append(info);
}
