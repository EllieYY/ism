#ifndef READERMANAGER_H
#define READERMANAGER_H

#include <QObject>

class ReaderManager : public QObject
{
    Q_OBJECT
public:
    explicit ReaderManager(QObject *parent = nullptr);

protected:
    void timerEvent(QTimerEvent* event);

signals:

};

#endif // READERMANAGER_H
