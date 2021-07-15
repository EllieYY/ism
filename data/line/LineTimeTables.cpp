#include "LineTimeTables.h"
#include "ISMTimeTable.h"

LineTimeTables::LineTimeTables(QString name, QString code, QString picPath,
                             QString color, QObject *parent) :
      LineInfo(name, code, picPath,color,parent)
{

}

QList<ISMTimeTable *> LineTimeTables::timeTable() const
{
    return m_timeTable;
}

void LineTimeTables::setTimeTable(const QList<ISMTimeTable *> &timeTable)
{
    m_timeTable = timeTable;
}

void LineTimeTables::addTimeTable(ISMTimeTable *timeTable)
{
    m_timeTable.append(timeTable);
}
