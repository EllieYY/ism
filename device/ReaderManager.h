#ifndef READERMANAGER_H
#define READERMANAGER_H

#include <QObject>

class ReaderManager : public QObject
{
    Q_OBJECT
public:
    explicit ReaderManager(QObject *parent = nullptr);
    ~ReaderManager();

public:
    void onStartDoingSomething(bool isOn);

protected:
    void timerEvent(QTimerEvent* event);


private:
    bool m_isOn;


signals:

};

#endif // READERMANAGER_H
