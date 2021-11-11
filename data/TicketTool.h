#ifndef TICKETTOOL_H
#define TICKETTOOL_H

#include <QObject>

class TicketTool : public QObject
{
    Q_OBJECT
public:
    explicit TicketTool(QObject *parent = nullptr);

signals:

};

#endif // TICKETTOOL_H
