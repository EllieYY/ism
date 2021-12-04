#include "ReaderManager.h"
#include <QThread>
#include <QDebug>

ReaderManager::ReaderManager(QObject *parent) : QObject(parent)
{
    m_isOn = false;
}

ReaderManager::~ReaderManager()
{
    qDebug() << "~DeviceManager()";
}

void ReaderManager::onStartDoingSomething(bool isOn)
{
    m_isOn = isOn;
}

void ReaderManager::timerEvent(QTimerEvent *event)
{
    qDebug() << "ReaderManager timer: " << QThread::currentThreadId();
//    if (m_rwState == 0) {
//        // 读写器心跳
//    }
//    if (m_coinState == 0) {
//    }
//    if (m_banknotes == 0) {
//    }
//    if (m_banknotesRe == 0) {
//    }

    if (m_isOn) {
        qDebug() << "ReaderManager on doing.....";
    }

}
