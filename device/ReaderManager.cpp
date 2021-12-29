#include "ReaderManager.h"
#include <QDebug>

ReaderManager::ReaderManager(QObject *parent) : QObject(parent)
{
    startTimer(1000);
}

void ReaderManager::timerEvent(QTimerEvent *event)
{
    qDebug() << "Thread心跳[ReaderManager]...";

}
