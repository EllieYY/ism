#include "DeviceManager.h"
#include "NC_ReaderLib.h"
#include "BIM2020.h"
#include "NCNetwork_Lib.h"
#include "MyHelper.h"

#define RETRY_COUNT 3

DeviceManager::DeviceManager(QObject *parent) : QObject(parent)
{
    // 初始状态：设备不可用
    m_rwState = 0;
    m_coinState = 0;
    m_banknotes = 0;
    m_banknotesRe = 0;
}

DeviceManager::~DeviceManager()
{

}

void DeviceManager::initDevice()
{


}

// 0 - 不可用   1 - 初始化成功  2 - 在线
void DeviceManager::initReader(int port, QString deviceIdStr)
{
    QByteArray devByteArray = MyHelper::hexStrToByte(deviceIdStr);
    BYTE* deviceId = (BYTE*)devByteArray.data();

    int tryTime = 0;
    while (tryTime++ < RETRY_COUNT) {
        // 打开串口
        int initRet = -1;
        int ret = (ushort)readerComOpen(port);
        if (ret == 0) {

            // 设备初始化、自检
            BYTE producter = 0;
            initRet = readerInit(deviceId, producter);
            if (initRet == 0) {
                m_rwState = 1;

                emit readerInitOk();
                break;
            }
        }
        logger()->info("[readerComOpen]读写器open={%1}, 端口号={%2}; [readerInit]={%3}",
                       ret, port, initRet);
    }
}


// 0 - 不可用   1 - 初始化成功  2 - 在线
void DeviceManager::initCashbox(int portCoin, int portBanknotes, int portBanknoteRe)
{
    int m_coinState;
    int m_banknotes;
    int m_banknotesRe;

    int tryTime = 0;
    while (tryTime++ < RETRY_COUNT) {
        // 端口：纸币、硬币、纸币找零器
        int retC = ConnectMachine(portBanknotes, portCoin, portBanknoteRe);
        if ((retC & 0x000F) == 0) {
            m_banknotes = 1;
        } else if ((retC & 0x00F0) == 0) {
            m_coinState = 1;
        } else if ((retC & 0x0F00) == 0) {
            m_banknotesRe = 1;
        }

        logger()->info("[ConnectMachine]钱箱初始化={%1}, 端口号：billPort={%2}, coinPort={%3}, changePort={%4}",
                       retC, portBanknotes, portCoin, portBanknoteRe);
    }

}

void DeviceManager::samInfo2Acc()
{
    BYTE* mtrSam = NULL;
    BYTE* octSam = NULL;
    BYTE* jtbSam = NULL;

    STATUS_INFO ReaderStatus = {0};
    BYTE ret = getStatus(&ReaderStatus);
    if (ret == 0) {
        int count = ReaderStatus.SAMCount;
        for (int i = 0; i < count; i++) {
            int type = ReaderStatus.SAMInfo[i].SAM_Type;
            BYTE* cardId = ReaderStatus.SAMInfo[i].SAMID;
            if (type == 0) {
                mtrSam = ReaderStatus.SAMInfo[i].SAMID;
            } else if (type == 1) {
                octSam = ReaderStatus.SAMInfo[i].SAMID;
            } else {
                jtbSam = ReaderStatus.SAMInfo[i].SAMID;
            }
        }

        if (mtrSam != NULL && octSam != NULL && jtbSam != NULL) {
            BYTE result = DeviceSAMInfo(mtrSam, octSam, jtbSam);

            QString mtrStr = QByteArray((char*)mtrSam, 6).toHex().toUpper();
            QString octStr = QByteArray((char*)octSam, 6).toHex().toUpper();
            QString jtbStr = QByteArray((char*)jtbSam, 6).toHex().toUpper();
            logger()->info("上传AFC{%4}；SAM卡号：地铁{%1}，洪城一卡通{%2}，交通部{%3}",
                           mtrStr, octStr, jtbStr, result);
        }
    }

    logger()->info("读写器获取SAM卡号信息{%1}", ret);
}
