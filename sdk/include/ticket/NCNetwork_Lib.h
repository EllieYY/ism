#pragma once

typedef unsigned long ulong;
typedef unsigned int UINT;
typedef unsigned short ushort;
typedef unsigned char BYTE;

#define WINDOWS
#define EXPORT

#include <malloc.h>
#ifdef WINDOWS
#ifdef EXPORT
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#include <stdlib.h>

#ifdef EXPORT
#define DLL_EXPORT __attribute__((visibility ("default")))
#else
#endif

#endif


#ifdef __cplusplus
extern "C" {
#endif 

#ifndef DLL_EXPORT
#define DLL_EXPORT
#endif

//  AFC标准中报文应答码/返回码
enum class responseCode
{
    MsgErrSuccess = 0x00,                    //成功
    MsgErrFormatError = 0x01,                //报文格式错误
    MsgErrErrorCmd = 0x02,                   //无效的消息分类/类型码
    MsgErrErrorNumValueRange = 0x03,         //无效的数值范围
    MsgErrTargetUnreach = 0x04,              //目标节点不可到达
    MsgErrTimeout = 0x05,                    //应答超时
    MsgErrErrorMultiPacket = 0x06,           //多包消息的固定部分不符
    MsgErrErrorRecordNumRange = 0x07,        //记录数超过消息包范围
    MsgErrRepeatRecord = 0x08,               //消息包中的记录重复
    MsgErrRecordNumDifferentWithPack = 0x09, //记录数与消息包长度不符
    MsgErrTimeError = 0x10,                  //交易明细或寄存器采集时间无效
    MsgErrVersionError = 0x11,               //参数版本不符
    MsgErrErrorNodeCode = 0x12,              //节点码非法
    MsgErrUndefined = 0xFF,                  //其它未定义的错误
};

//  自定义错误返回码
#define ERR_LOAD_LIB 0x31   //库加载失败
#define ERR_SEND 0x32       //发送失败（区别于连接失败）
#define ERR_SOCKET_ERR 0x33 //连接异常中断
#define ERR_LISTEN_ERR 0x34 //初始化服务端时监听异常

    enum class FileType
    {
        AFC_ULTradeFile = 0x01,
        AFC_MetroTradeFile = 0x02,
        AFC_OCTTradeFile = 0x03,
        AFC_RegisteFile = 0x04,
        AFC_ParamFile = 0x05,
        AFC_FTPAuditFile = 0x06,
        AFC_AccountFile = 0x07,
        AFC_Program = 0x08,
        AFC_RevenueFile = 0x09,
        // AFC_QRTradeFile = 0x0B,
        // AFC_QPTradeFile = 0x0C,
        AFC_TUTradeFile = 0x0D,
    };

    enum class FTPResultCode
    {
        FTPErrSuccess = 0x00,
        FTPErrLogin = 0x01,
        FTPErrPathNotExist = 0x02,
        FTPErrFileNotExist = 0x03,
        FTPErrMD5unmatch = 0x04,
        FTPErrFormt = 0x05,
        FTPErrRecordCount = 0x06,
        FTPErrUndefined = 0xFF,
    };

    enum class deviceCommand
    {
        QueryRegisterData = 0x01,
        QueryDeviceState = 0x02,
        QueryMoneyBoxInfo = 0x03,
        QueryTokenBoxInfo = 0x04,
        DeviceOpen = 0x05,
        DeviceClose = 0x06,
        RGOnlyEnter = 0x07,
        RGOnlyOut = 0x08,
        RGTwoWayMod = 0x09,
        StartTestMode = 0x0A,
        StopTestMode = 0x0B,
        CleanCoin = 0x0C,
        NotAcceptCash = 0x0D,
        AcceptCash = 0x0E,
        NotAcceptCoin = 0x0F,
        AcceptCoin = 0x10,
        ForbidRecharge = 0x1A,
        AllowedRecharge = 0x1B,
        DisenableSell = 0x1C,
        EnableSell = 0x1D,
        DisenableCashChange = 0x1E,
        EnableCashChange = 0x1F,
        CleanToken = 0x20,
    };

    typedef struct
    {
        BYTE paramCode[2];
        BYTE paramType;
        BYTE paramVer[4];
    } AFC_4001_PKG_BODY_R, *pAFC_4001_PKG_BODY_R;

    typedef struct
    {
        BYTE PartID[4];
        BYTE AppType;
        BYTE AppVer[4];
        BYTE reserve1[4];
        BYTE reserve2[4];
    } AFC_9005_PKG_BODY_R, *pAFC_9005_PKG_BODY_R;

    typedef struct 
    {
        BYTE messageType[2];
        BYTE messageBody[256];
        int recordCount;
    } messageBuff, *pmessageBuff;

    DLL_EXPORT BYTE initNetworkLib(BYTE *deviceID, BYTE *scID, char *scIP, UINT scPort, char *localIP, UINT localPort);
    DLL_EXPORT void getLibVersion(char *verInfo);
    DLL_EXPORT UINT getLastMsgSeq(void);
    DLL_EXPORT void setLastMsgSeq(UINT MsgSeq);
    // 获取缓存中最新一条要处理的SC消息的序号
    DLL_EXPORT UINT getLastNeedDealSeq();
    // 获取缓存中最新一条要回复的SC消息的序号
    DLL_EXPORT UINT getLastNeedResponseSeq();
    // 获取缓存中指定序号的要处理SC消息内容
    DLL_EXPORT void getSpecNeedDeal(UINT countPointer, pmessageBuff messageCache);
    // 获取缓存中指定序号的要回复SC消息内容
    DLL_EXPORT void getSpecNeedResponse(UINT countPointer, pmessageBuff messageCache);
    DLL_EXPORT BYTE DegradeModeNotify(BYTE *modeCode, BYTE *modeApplyTime);
    DLL_EXPORT BYTE DeviceState(BYTE status1, BYTE *event,int eventCount);
    DLL_EXPORT BYTE ParamAppliedNotify(BYTE *paramType, BYTE *versionOld, BYTE *versionNew, BYTE *applyTime);
    DLL_EXPORT BYTE FileDownloadNotify(BYTE fileType, BYTE *fileName, BYTE *md5);
    DLL_EXPORT BYTE FileDownloadConfirm(BYTE fileType, BYTE *fileName, BYTE result);
    DLL_EXPORT BYTE TcpLinkTest(void);
    DLL_EXPORT BYTE DeviceSAMInfo(BYTE *mtrSam, BYTE *octSam, BYTE *jtbSam);
    DLL_EXPORT BYTE OperatorAction(BYTE *operatorID, BYTE event, BYTE operatorType);
    DLL_EXPORT BYTE SoftwareUpdateNotify(BYTE applicationType, BYTE *version);
    DLL_EXPORT BYTE QueryDegradeMode(BYTE *modeCode, BYTE *modeApplyTime);
    // MessageResponse 为 paramCount 个 AFC_4001_PKG_BODY_R 结构体组织成的数据
    DLL_EXPORT BYTE ParamQuery(BYTE *MessageResponse, int paramCount);
    // MessageResponse 为 paramCount 个 AFC_9005_PKG_BODY_R 结构体组织成的数据
    DLL_EXPORT BYTE SoftVersionQuery(BYTE *MessageBody, int reportCount);

#ifdef __cplusplus
}
#endif 





