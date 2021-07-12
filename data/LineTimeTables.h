#ifndef LINETIMETABLE_H
#define LINETIMETABLE_H

#include "LineInfo.h"

class ISMTimeTable;
class LineTimeTables : public LineInfo
{
    Q_OBJECT
public:
    explicit LineTimeTables(QString name, QString picPath, QString color,
                           QObject *parent = nullptr);

    QList<ISMTimeTable *> timeTable() const;
    void setTimeTable(const QList<ISMTimeTable *> &timeTable);
    void addTimeTable(ISMTimeTable* timeTable);

private:
    QList<ISMTimeTable*> m_timeTable;  // 时刻表


signals:

};

#endif // LINETIMETABLE_H
