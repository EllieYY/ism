#include "AFCTaskThread.h"
#include "NCNetwork_Lib.h"
#include "MyHelper.h"
#include <QDebug>

AFCTaskThread::AFCTaskThread(QObject *parent) : QThread(parent)
{
    m_seq = -1;
    m_respSeq = -1;
}


//int seq = getLastNeedDealSeq();
//if(m_seq != seq) {
//    logger()->info("getLastNeedDealSeq = {%1}, lastSeq = {%2}", seq, m_seq);
//    m_seq = seq;

//    // 获取消息
//    messageBuff buff = {0};
//    getSpecNeedDeal(m_seq, buff);
//    logger()->info("getSpecNeedDeal = {%1}", m_seq);
//    //TODO:消息处理
//}


void AFCTaskThread::run()
{
    while(1) {
        int respSeq = getLastNeedResponseSeq();
        if(m_respSeq != respSeq) {
            logger()->info("getLastNeedResponseSeq = {%1}, lastSeq = {%2}", respSeq, m_respSeq);

            m_respSeq = respSeq;

            // 获取消息
            messageBuff buff = {0};
            getSpecNeedResponse(m_respSeq, buff);

            QString type = QByteArray((char*)buff.messageType, 2).toHex();
            qDebug() << "type: " << type;

            // 消息回复
            afcResp(buff.messageType, buff.messageBody);
        }
    }
}

void AFCTaskThread::afcResp(uchar *type, uchar *body)
{
//    memcpy(bRecv+1,(uchar*)&UpdateTradeData, sizeof(UpdateTradeData));
    bool ok;
    QByteArray array_data;
    array_data.append((char*)&type, 2);
    long typeNum = array_data.toLong(&ok, 16);
    if (typeNum == 0x4001) {
        make4001Resp();
    } else if (typeNum == 0x9005) {
        make9005Resp();
    }
}

void AFCTaskThread::make4001Resp()
{
    AFC_4001_PKG_BODY_R param1 = {0};
    param1.paramCode[1] = 0x10; // 费率表
    param1.paramCode[0] = 0x06;
    param1.paramType = 0x01;    // 当前参数
    param1.paramVer[3] = 0x20;  // 版本号
    param1.paramVer[2] = 0x21;
    param1.paramVer[1] = 0x11;
    param1.paramVer[0] = 0x01;

    AFC_4001_PKG_BODY_R param2 = {0};
    param2.paramCode[1] = 0x10; // 车站名称表
    param2.paramCode[0] = 0x03;
    param2.paramType = 0x01;    // 当前参数
    param2.paramVer[3] = 0x20;  // 版本号
    param2.paramVer[2] = 0x21;
    param2.paramVer[1] = 0x11;
    param2.paramVer[0] = 0x01;

    QList<AFC_4001_PKG_BODY_R> paramList;
    paramList.append(param1);
    paramList.append(param2);

    int paramNum = paramList.size();
    int paramSize = sizeof(AFC_4001_PKG_BODY_R);
    int length = paramNum * paramSize;
    BYTE *p = new BYTE[length];

    for (int i = 0; i < paramNum; i++) {
        int index = i * paramSize;
        memmove(p + index, &paramList.at(i), paramSize);
    }

    ParamQuery(p, paramNum);
    logger()->info("make 4001 resp");
}

void AFCTaskThread::make9005Resp()
{
    AFC_9005_PKG_BODY_R param1 = {0};
    param1.AppType = 0x09;     // BOM
    param1.AppVer[3] = 0x00;   // 版本号
    param1.AppVer[2] = 0x00;
    param1.AppVer[1] = 0x00;
    param1.AppVer[0] = 0x02;

    param1.PartID[3] = 0x00;     // 部件ID
    param1.PartID[2] = 0x00;
    param1.PartID[1] = 0x00;
    param1.PartID[0] = 0x03;

    QList<AFC_9005_PKG_BODY_R> paramList;
    paramList.append(param1);

    int paramNum = paramList.size();
    int paramSize = sizeof(AFC_9005_PKG_BODY_R);
    int length = paramNum * paramSize;
    BYTE *p = new BYTE[length];

    for (int i = 0; i < paramNum; i++) {
        int index = i * paramSize;
        memmove(p + index, &paramList.at(i), paramSize);
    }

    SoftVersionQuery(p, paramNum);
    logger()->info("make 9005 resp");
}
