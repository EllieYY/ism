#include "AFCTaskThread.h"
#include "NCNetwork_Lib.h"
#include "MyHelper.h"
#include <QDebug>
#include "CommonHead.h"
#include "DataCenter.h"
#include <time.h>
#include "SettingCenter.h"
#include "BomParamVersionInfo.h"

AFCTaskThread::AFCTaskThread(QObject *parent) : QThread(parent)
{
    m_dealSeq = -1;
    m_respSeq = -1;
}

void AFCTaskThread::run()
{
    while(1) {
        respCheck();
        dealCheck();
    }
}

void AFCTaskThread::dealCheck()
{
    int respSeq = getLastNeedDealSeq();
    if(m_dealSeq != respSeq) {
        logger()->info("getLastNeedDealSeq = {%1}, lastSeq = {%2}", respSeq, m_respSeq);
        m_dealSeq = respSeq;

        // 获取消息
        messageBuff buff = {0};
        getSpecNeedDeal(m_dealSeq, &buff);

        // 消息回复
        bool ok;
        int type = QByteArray((char*)buff.messageType, 2).toHex().toInt(&ok, 16);
        afcResp(type, buff.messageBody, buff.recordCount);
    }
}

void AFCTaskThread::respCheck()
{
    int respSeq = getLastNeedResponseSeq();
    if(m_respSeq != respSeq) {
        logger()->info("getLastNeedResponseSeq = {%1}, lastSeq = {%2}", respSeq, m_respSeq);
        m_respSeq = respSeq;

        // 获取消息
        messageBuff buff = {0};
        getSpecNeedResponse(m_respSeq, &buff);

        // 消息回复
        bool ok;
        int type = QByteArray((char*)buff.messageType, 2).toHex().toInt(&ok, 16);
        afcResp(type, buff.messageBody, buff.recordCount);
    }
}

void AFCTaskThread::afcResp(int type, uchar *body, int count)
{
    if (type == 0x4001) {    // 参数查询
        make4001Resp();
    } else if (type == 0x9005) {   // 软件/部件版本查询
        make9005Resp();
    } else if (type == 0x2000) {   // 模式命令
        parse2000(body);
    } else if (type == 0x3000) {    // 设备控制命令
        parse3000(body);
    } else if (type == 0x4000) {    // 参数同步
        parse4000(body, count);
    } else if (type == 0x7003) {    // 交易文件索取

    } else if (type == 0x9001) {    // 强制时间同步
        parse9001(body);
    } else if (type == 0x9004) {    // 软件、部件版本更新
        parse9004(body);
    }

    // TODO：文件传输命令待整理


}

void AFCTaskThread::make4001Resp()
{
    QList<AFC_4001_PKG_BODY_R> paramList;
    QList<BomParamVersionInfo*> list = SettingCenter::getThis()->getParamVersionInfo();
    for (BomParamVersionInfo* item : list) {
        AFC_4001_PKG_BODY_R param = {0};
        param.paramType = 0x01;    // 当前参数

        QByteArray typeArray = MyHelper::intToBytes(item->type(), 2);
        BYTE* typeByte = (BYTE*)typeArray.data();
        memcpy(param.paramCode, typeByte, 2);

        QByteArray versionArray = MyHelper::intToBytes(item->version(), 4);
        BYTE* versionByte = (BYTE*)versionArray.data();
        memcpy(param.paramVer, versionByte, 4);

        paramList.append(param);
    }

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
    QString deviceIdStr = DataCenter::getThis()->getDeviceId();
    QString versionStr = DataCenter::getThis()->getReaderVersion();

    AFC_9005_PKG_BODY_R param = {0};
    param.AppType = 0x01;         // 读卡器
    MyHelper::hexStrToByte(deviceIdStr, 4, param.PartID);  // 部件ID
    MyHelper::hexStrToByte(versionStr, 4, param.AppVer);   // 读写器版本号

    QList<AFC_9005_PKG_BODY_R> paramList;
    paramList.append(param);

    int paramNum = paramList.size();
    int paramSize = sizeof(AFC_9005_PKG_BODY_R);
    int length = paramNum * paramSize;
    BYTE *p = new BYTE[length];

    for (int i = 0; i < paramNum; i++) {
        int index = i * paramSize;
        memmove(p + index, &paramList.at(i), paramSize);
    }

    int ret = SoftVersionQuery(p, paramNum);

    QByteArray paramArray;
    paramArray.append((char*)&param, sizeof(AFC_9005_PKG_BODY_R));
    QString msg = paramArray.toHex().toUpper();
    logger()->info("make 9005 resp, %1, param = [%2]", ret, msg);
}

void AFCTaskThread::parse2000(uchar *msg)
{
//    pAFC_2000_PKG_BODY body = (pAFC_2000_PKG_BODY)msg;
    QByteArray array = QByteArray((char*)msg, sizeof(AFC_2000_PKG_BODY));
    pAFC_2000_PKG_BODY info = (pAFC_2000_PKG_BODY)array.data();

    // 运行模式码
    bool ok;
    int mode = QByteArray((char*)info->modeCode, 2).toHex().toInt(&ok, 16);

    // 写入DataCenter
    DataCenter::getThis()->setStationMode(mode);

    // 日志记录：
    QString msgStr = array.toHex().toUpper();
    QString timeStr = QByteArray((char*)info->currentTime, 7).toHex();
    QString codeStr = QByteArray((char*)info->modeCode, 2).toHex();
    QString operatorStr = QByteArray((char*)info->operatorID, 4).toHex();
    logger()->info("[2000h]msg=%1, time=%2, code=%3, operatorId=%4", msgStr, timeStr, codeStr, operatorStr);
}

// 暂时只处理设备状态上报 02h
void AFCTaskThread::parse3000(uchar *msg)
{
    QByteArray array = QByteArray((char*)msg, sizeof(AFC_3000_PKG_BODY));
    pAFC_3000_PKG_BODY info = (pAFC_3000_PKG_BODY)array.data();

    // 设备状态上报
    int cmd = info->cmdCode;
    if (cmd == 0x02) {
        DataCenter::getThis()->deviceState2afc();
    }

    // 日志记录
    QString msgStr = array.toHex().toUpper();
    QString senderStr = QByteArray((char*)info->sendID, 4).toHex();
    QString receiverStr = QByteArray((char*)info->receiveID, 4).toHex();
    QString cmdStr = QString::number(cmd, 16);
    QString operatorId = QByteArray((char*)info->operatorID, 4).toHex();
    logger()->info("[3000h]msg=%1, sender=%2, receiver=%3, cmd=%4, operatorId=%5",
                   msgStr, senderStr, receiverStr, cmdStr, operatorId);
}

/* 处理逻辑：
 * 接受请求消息后，通过登录消息发送方的FTP目录获取参数版本的文件名，
 * 与系统中正在使用的参数版本进行核对，根据同步类型，确定是否需要下载参数
*/
void AFCTaskThread::parse4000(uchar *msg, int count)
{
    qDebug() << "count = " << count;
    QByteArray array = QByteArray((char*)msg, sizeof(AFC_4000_PKG_BODY));
    QString targetStr = array.mid(0, 4).toHex();

    QStringList paramList;
    count = count > MAX_PARAM_NUM ? MAX_PARAM_NUM : count;
    for (int i = 0; i < count; i++) {
        QString paramType = array.mid(2 * i + 5, 2).toHex();
        paramList.append(paramType);

        // TODO:获取目录 - 参数版本比对

        // TODO: FTP下载 - 解析 - 更新

    }

    // 日志记录
    QString msgStr = array.toHex().toUpper();
    QString typeStr = array.mid(4, 1).toHex();
    logger()->info("[4000h]msg=%1, target=%2, type=%3, paramList=[%4]",
                   msgStr, targetStr, typeStr, paramList.join(","));
}

// 系统时间同步
void AFCTaskThread::parse9001(uchar *msg)
{
    QByteArray array = QByteArray((char*)msg, sizeof(AFC_9001_PKG_BODY));
    pAFC_9001_PKG_BODY info = (pAFC_9001_PKG_BODY)array.data();

    // 系统时间
    QString timeStr = QByteArray((char*)info->currentTime, 7).toHex();
    QDateTime datetime = QDateTime::fromString(timeStr, "yyyyMMddHHmmss");

    // 时间同步
    SYSTEMTIME   st;
    GetLocalTime(&st);

    QDate date = datetime.date();
    st.wYear = date.year();
    st.wMonth = date.month();
    st.wDay = date.day();

    QTime time = datetime.time();
    st.wHour = time.hour();
    st.wMinute = time.minute();
    st.wSecond = time.second();
    SetLocalTime(&st);

    // 日志记录
    QString msgStr = array.toHex().toUpper();
    logger()->info("[9001h]msg=%1, datetime=%2", msgStr, timeStr);
}


/* 软件、部件版本更新:
 * 主动向上级节点以FTP方式获取更新文件，并进行更新
 * 目前只处理读写器程序的更新
*/
void AFCTaskThread::parse9004(uchar *msg)
{
    QByteArray array = QByteArray((char*)msg, sizeof(AFC_9004_PKG_BODY));
    pAFC_9004_PKG_BODY info = (pAFC_9004_PKG_BODY)array.data();

    int type = info->AppType;
    QByteArray nameArray = QByteArray((char*)info->AppName, 50);
    QString fileName = MyHelper::getCorrectUnicode(nameArray);
    if (type == 0x01) {

        // TODO:下载程序文件，并更新读写器程序，重新初始化读写器

    }

    // 日志记录
    QString msgStr = array.toHex().toUpper();
    logger()->info("[9004h]msg=%1, type=%2, filename=%3", msgStr, type, fileName);
}
