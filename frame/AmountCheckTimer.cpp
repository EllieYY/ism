#include "AmountCheckTimer.h"
#include <QDebug>
#include <QDateTime>
#include "BIM2020.h"

#define MIN_5 5*60

AmountCheckTimer::AmountCheckTimer(QObject *parent)
    : AsyncTimer(parent)
{
    m_startTime = -1;
}

AmountCheckTimer::~AmountCheckTimer()
{
    // 注意：若在此类run()中使用了成员变量，则在此类对象析构前，
    // 必须调用waitForQuit()让线程先退出，否则可能出现此类成员变量已被析构，
    // 但是线程仍然还在运行run()的情况，此时访问成员变量出现段错误。
    // 建议：默认在子类析构函数中调用waitForQuit()。
    waitForQuit();
}

void AmountCheckTimer::run()
{

     qDebug() << "run0...";

//    m_lock.lock();
    long currentTime = QDateTime::currentSecsSinceEpoch();
    if (m_startTime <= 0) {
        m_startTime = currentTime;
    }


    qDebug() << "run1...";
    if (pauseFlag) {
        qDebug() << "pause...";
        m_startTime = currentTime;
        return;
    }
    long diff = currentTime - m_startTime;

    qDebug() << "cheking...";



    // 投币完成检测
    int bankNoteCount = 0;
    int coinCount = 0;
    int ret = CheckCoinStatus(&bankNoteCount, &coinCount);
    emit checkState(ret, bankNoteCount, coinCount);
    if (ret == 0) {
        m_startTime = -1;
        pauseFlag = true;
        emit receiveOk(bankNoteCount, coinCount);
    }

    // 超时：调用停止投币接口
    if (diff > MIN_5) {
        int bankNotes = 0;
        int coins = 0;
        StopPutCoin(&bankNotes, &coins);

        m_startTime = -1;
        emit timeoutReceive(bankNotes, coins);
    }

//    m_lock.unlock();
}
