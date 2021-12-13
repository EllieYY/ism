#pragma once

#include "typedefine.h"

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




#define ZONE_FREE   0x00
#define ZONE_PAY    0x01

#define READER_CMD_INIT  0x01
#define READER_CMD_ANT   0x02
#define READER_CMD_STATUS  0x03
#define READER_CMD_VERSION  0x04
#define READER_CMD_DOWNLOAD  0x11
#define READER_CMD_ACTIVE_APP  0x12
#define READER_CMD_PARAM_VER  0x13
#define READER_CMD_SEARCH  0x21
#define READER_CMD_SCANER  0x91

    //进出闸
#define READER_CMD_UL_ENTRY  0x33
#define READER_CMD_UL_EXIT  0x34
#define READER_CMD_MTR_ENTRY  0x49
#define READER_CMD_MTR_EXIT  0x4A
#define READER_CMD_HCT_ENTRY  0x53
#define READER_CMD_HCT_EXIT  0x54

//费用计算
#define READER_CMD_FARE_CAL  0x30
//发售
#define READER_CMD_UL_ISSUE  0x31
//分析
#define READER_CMD_UL_ANALYSE  0x36
#define READER_CMD_MTR_ANALYSE  0x4B
#define READER_CMD_HCT_ANALYSE  0x51
#define READER_CMD_JTB_ANALYSE  0x71
#define READER_CMD_UNI_ANALYSE  0x81

//历史
#define READER_CMD_MTR_HISTORY  0x4C
#define READER_CMD_HCT_HISTORY  0x57

//更新
#define READER_CMD_UL_UPDATE  0x32
#define READER_CMD_MTR_UPDATE  0x48
#define READER_CMD_HCT_UPDATE  0x52

//UL退款
#define READER_CMD_UL_REFUND  0x35

#define MAX_DATA_FRAME_BYTES	1023

#define FILE_APP_TYPE	0x08
#define FILE_PARAM_TYPE 0x05


#define CAL_FARE_TO_EXIT				0x01
#define CAL_FARE_EXIT_ONLY_TICKET		0x02
#define CAL_FARE_FREE_AREA_PAY			0x03
#define CAL_FARE_PAY_AREA_NO_ENTRY		0x04
#define CAL_FARE_PAY_AREA_TIMEOUT		0x05
#define CAL_FARE_PAY_AREA_FAREOUT		0x06
#define CAL_FARE_REFUND					0x07
#define CAL_FARE_TICKET_SALE			0x08
#define CAL_FARE_TIMEOUT_FAREOUT		0x09

    typedef struct
    {
        BYTE phyID[8];
        BYTE LogicID[8];
        BYTE RFU[10];
    }SEARCHCARD_INFO, * PSEARCHCARD_INFO;

    typedef struct
    {
        BYTE DevTradeSerial[4];//设备交易流水
        BYTE StationMode[2];//车站模式
        BYTE CardType;//车票类型
        BYTE TestMode;//测试模式
        BYTE ExitTicket;//出站票标志
        BYTE PayType;//支付方式
        BYTE CardValue[4];//票面金额
        BYTE TargetStation[2];//目标车站
        BYTE OperatorID[4];//操作员编号
        BYTE OCTLogicID[8];
        BYTE RFU[10];
    }ISSUECARD_IN, * PISSUECARD_IN;

    typedef struct
    {
        BYTE CardType;//票卡类型
        BYTE CalMode;//计费操作码
        BYTE TargetStation[2];//目标车站
        BYTE RFU[10];
    }FARECAL_IN, * PFARECAL_IN;

    typedef struct
    {
        BYTE Amount[4];//票价
        BYTE Deposit[4];//押金
        BYTE Discount[4];//折旧费
        BYTE ReturnTkFare[4];//退票手续费
        BYTE RegionCode;//区域码
        BYTE RFU[9];
    }FARECAL_OUT, * PFARECAL_OUT;

    typedef enum
    {
        UPDATE_NOFARE_EXIT, //1 免费出站更新（在付费区）
        UPDATE_FARE_EXIT, //2 付费出站更新 （在付费区）
        UPDATE_ENTRY, //3 进站更新 （在非付费区）
        UPDATE_OUT_RANGE, //4 超程更新
        UPDATE_OUT_TIME, //5 超时更新
        UPDATE_OUT_RANGE_TIME, //6 超时超程更新
        UPDATE_NOLOCAL_RECORD
    }BOM_UPDATE_TYPE;//更新类型

    typedef struct
    {
        BYTE DevTradeSerial[4];//设备交易流水
        BYTE StationMode[2];//车站模式
        BYTE CardType;//车票类型
        BYTE TestMode;//测试模式
        BYTE PayType;//支付方式
        BYTE Amount[4];//交易金额
        BYTE UpdateType;//更新方式
        BYTE OperatorID[4];//操作员编号
        BYTE UpdateStation[2];//进站车站
        BYTE RFU[10];
    }UPDATE_CARD_IN, * PUPDATE_CARD_IN;

    typedef struct
    {
        BYTE DevTradeSerial[4];//设备交易流水
        BYTE StationMode[2];//车站模式
        BYTE CardType;//车票类型
        BYTE TestMode;//测试模式
        BYTE PayType;//支付方式
        BYTE Amount[4];//交易金额
        BYTE OperatorID[4];//操作员编号
        BYTE RFU[10];
    }UL_REFUND_IN, * PUL_REFUND_IN;

    typedef struct
    {
        BYTE ICType;//芯片类型
        BYTE CardInfo[512];
    }ANALYSECARD_RESP, * PANALYSECARD_RESP;

    typedef struct
    {
        BYTE TradeSerialNum[2];
        BYTE TradeDate[7];
        BYTE TradeAmount[4];
        BYTE TradeType;
        BYTE OverDrartLimit[3];
        BYTE PsamID[6];
    }CARD_HISTORY, * PCARD_HISTORY;

#define MAX_HISTORY_NUM 10
    typedef struct
    {
        BYTE HistoryCount;//交易条数
        CARD_HISTORY HistoryInfo[MAX_HISTORY_NUM];
    }HISTORY_RESP, * PHISTORY_RESP;

    typedef struct
    {
        BYTE HardVersion[4];
        BYTE SoftVersion[2];
    }VERSION_INFO, * PVERSION_INFO;

    typedef struct
    {
        BYTE SAM_Type;//SAM卡类型
        BYTE SAM_Status;//SAM卡状态
        BYTE SAMID[6];//SAM卡号
        BYTE Baud[2];//波特率

    }SAM_INFO, * PSAM_INFO;

#define MAX_SAM_NUM 8
    typedef struct
    {
        BYTE Rf1Status;
        BYTE Rf2Status;
        BYTE WordMode;//读写器工作模式
        BYTE SAMCount;//卡槽记录数
        SAM_INFO SAMInfo[MAX_SAM_NUM];
        BYTE QRStatus;

    }STATUS_INFO, * PSTATUS_INFO;

    typedef struct
    {
        BYTE ParamCode[2];
        BYTE ParamVer[4];
    }PARAM_INFO, * PPARAM_INFO;

#define MAX_PARAM_NUM 40
    typedef struct
    {
        BYTE ParamCount;
        PARAM_INFO paramVerInfo[MAX_PARAM_NUM];
    }PARAM_VERSION, * PPARAM_VERSION;

    typedef struct {
        BYTE allowUpdate;	//0：不可更新1：可以更新
        BYTE checkStatus;	//B0：发行区校验码正确 B1：发售区校验码正确 B2：进站区校验码正确
        BYTE logicID[8];		//逻辑卡号
        BYTE cardType;		//
        BYTE testMode;		//0：正式模式1：测试模式
        BYTE isSouvenir;		//0：非纪念票1：纪念票
        BYTE isExitOnly;		//0：非出站票 1：出站票
        BYTE soldDate[4];	//发售日期yyyymmdd
        BYTE parValue;		//角
        BYTE expireDays[2];	//有效期天数
        BYTE saleDevID[4];	//补票设备编号
        BYTE saleTm[7];		//售补票时间
        BYTE tradeCount[2];	//票计数器
        BYTE status;			//Bit2bit1bit0为票卡状态
                                    //	000	初始化
                                    //	001	售票
                                    //	010	进站
                                    //	011	出站
                                    //	100	列车故障模式出站
                                    //	101	注销
                                    //	bit4 表示前一次是否有超程更新
                                    //  bit5 - 表示前一次是否有超时更新
                                    //	在BOM / TVM界面显示时，只需要显示bit2 - 0即票卡状态
        BYTE entryDevID[4];	//进站设备编号
        BYTE entryTm[7];		//进站时间
        BYTE exitDevID[4];	//出站设备编号
        BYTE exityTm[7];		//出站票时间
        BYTE opt[4];			//如果车票主类型05且该值非0则为日次票首次使用时间，此次为北京时间从1970年1月1号0时0分0秒开始的秒数
        BYTE ref[16];		//预留
    }ULAnalyseOut, * pULAnalyseOut;

    typedef struct {
        BYTE allowUpdate;	//0：不可更新1：可以更新
        BYTE phyID[8];		//逻辑卡号
        BYTE balance[4];	//余额/余次
        BYTE nextMothBalance[4];	//下月钱包余次 仅限月票，当符合下月有效期则有月票次数，否则填0

        BYTE issueCode[2];				// 发卡方代码 2 bcd 0000h
        BYTE cityCode[2];				// 城市代码 2 bcd
        BYTE tradeCode[2];				// 行业代码 2 bcd 5320h
        BYTE serialNum[8];			// 应用序列号 卡片唯一号 8 HEX （1字节票卡芯片类型+4字节初始化日期+1字节初始化批次+2字节初始化流水号）
        BYTE initSamID[6];				// 初始化设备SAM卡号 HEX 
        BYTE initDate[4];				// 初始化日期 BCD YYYYMMDD

        BYTE cardType;					// 票种 HEX （80h-8Fh：计程票，90h-93h：计次票，94h-97h：月票，98h-9Fh：公务票）
        BYTE testTicket;					// 测试卡 HEX，取卡标志B7
        BYTE regTicket;					// 记名卡	1	HEX，取卡标志B6
        BYTE souTicket;					// 纪念卡	1	HEX，取卡标志B5
        BYTE cardStatus;					// 卡状态	1	HEX，取卡标志B4B3B2B1B0
                                        // 0：未初始化1：初始化；
                                        // 2：已发售；3：已锁卡；
                                        // 4：已退票。

        BYTE validityDayFormat;			// 有效期格式（0：卡内记录有效期，1：有效期由系统参数确定，第一次在AGM上使用后开始计算有效期，2：每次充值时在对应月票目录（3F02或3F03）中写入有效期）
        BYTE validityDay[4];				// 有效期
        BYTE saleDay[4];					// 发售日期 BCD YYYYMMDD
        BYTE saleDevSN[4];				// 发售设备
        BYTE saleSamID[6];				// 发售设备SAM卡号
        BYTE deposit[4];					// 票卡押金 HEX(M)    分 
        BYTE HolderTypeFlag;			// 持卡人类型标识 （0：普通乘客，1：地铁员工）
        BYTE HolderWorkFlag;			// 持卡人职工标识 （暂不启用，填0）
        BYTE CardholderName[20];     	// 持卡人姓名 20 ASCII    
        BYTE HolderCardNum[32];     	// 持卡人证件号码 32 ASCII
        BYTE HolderCardType;			// 持卡人证件类型 （0：身份证，1：社保卡，2：学生证，3：残疾人证，4：军官证，5：护照）
        BYTE LimitsFlag;				// 范围标志 （0：部分线路的所有车站，x：某x条线路的部分车站）
        BYTE LineBit[4];				// 线路（车站）位图1
        BYTE StationBit[4];     		// 车站位图2    
        BYTE StaffNum[8];     			// 员工号   ASCII   

        BYTE appStartFlag;				// 应用启用标志 bcd
        BYTE appVersion;					// 应用版本 bcd
        BYTE appSerialNum[8];			// 应用序列号 卡片唯一号 8 hex (2：城市代码BCD 1：卡主类型BCD 1：卡子类型BCD 4：发行流水HEX)
        BYTE appStartDate[4];			// 应用启用日期 4 bcd yyyymmdd
        BYTE appAvailDate[4];			// 应用有效日期 4 bcd yyyymmdd

        BYTE statusFlag;               // 状态标志 (bit5：超时更新；bit4：超乘更新；bit3-bit1：0初始化，1售票，2进站，3出站，4锁卡，5注销；bit0：偶校验位)
        BYTE firstUseDate[7];		  // 首次启用时间 bcd	YYYYMMDDHHMMSS
        BYTE dayTradeNum;			  // 当日累计乘车次数 HEX(M)	
        BYTE enterTradeTime[7];		  // 进站时间 bcd	YYYYMMDDHHMMSS
        BYTE enterTradeDev[4];		  // 进站设备 bcd
        BYTE monTradeNum1[2];		  // 当月累计次数 HEX(M)
        BYTE monTradeValue1[3];		  // 当月累计金额 HEX(M) 

        BYTE exitTradeTime[7];		  // 出站时间 bcd	YYYYMMDDHHMMSS
        BYTE exitTradeDev[4];		  // 出站设备 bcd
        BYTE monTradeNum2[2];		  // 当月累计次数 HEX(M)
        BYTE monTradeValue2[3];		  // 当月累计金额 HEX(M) 	

        BYTE updataTime[7];			  // 更新时间 bcd	YYYYMMDDHHMMSS
        BYTE updataTradeDev[4];		  // 更新设备 HEX
        BYTE dayUpdataNum;			  // 当日更新次数	

    }MetroCPUAnalyseOut, * pMetroCPUAnalyseOut;

    typedef struct {
        BYTE allowUpdate;			//0：不可更新1：可以更新
        BYTE phyID[8];				//8字节物理卡号,前补0x00
        BYTE logicID[8];				//逻辑卡号
        BYTE balance[4];				// 票卡余额或次数
        BYTE ticketType;				// 车票类型
        BYTE mastertype;				// 卡主类型 bcd
        BYTE subType;				// 卡子类型 bcd
        BYTE logicID2[8];			//逻辑卡号
        BYTE issueCode[2];			// 发卡方代码 2 bcd
        BYTE cityCode[2];			// 城市代码 2 bcd
        BYTE tradeCode[2];			// 行业代码 2 bcd
        BYTE cityUnion[2];			// 互联互通标识 BCD
        BYTE appStartDate[4];		// 应用启用日期 4 bcd yyyymmdd
        BYTE appAvailDate[4];		// 应用有效日期 4 bcd yyyymmdd
        BYTE appStartFlag;			// 应用启用标志 bcd
        //复合记录
        //3f01-19
        BYTE AppLockFlag;				// 应用锁定标志位 1锁定
        BYTE StatusFlag;					// 状态标志 (实际使用B2B1B0，其中B2：是否做了超时更新 1-是0-否 B1：是否做了超程更新1-是0-否 B0：1-进站 0-出站)
        BYTE TradeTime[6];				// 进站时间 bcd	YYMMDDHHMMSS
        BYTE TradeStations[2];			// 进站车站编号 bcd
        BYTE ExitTime[6];				// 出站时间 bcd	YYMMDDHHMMSS
        BYTE ExitStations[2];			// 出站车站编号 bcd	
        BYTE UpStations[2];				// 更新车站编号 bcd	
        BYTE TradeValue[2];				// 最近交易金额(单位=分) HEX(M)
        BYTE DayTradeNum;				// 当日累计乘车次数 HEX(M)
        BYTE MonTradeNum[2];				// 当月累计乘车次数 HEX(M)
        BYTE MonTradeValue[3];			// 当月累计乘车金额 HEX(M)  
        BYTE ref[40];
    }OCTAnalyseOut, * pOCTAnalyseOut;

    typedef struct {
        BYTE allowUpdate;			//0：不可更新1：可以更新
        BYTE phyID[8];			//8字节物理卡号,前补0x00
        BYTE logicID[10];			//逻辑卡号
        BYTE balance[4];			// 票卡余额
        BYTE ticketType;
        BYTE mainType;			// 车票类型
        BYTE subType;			// 卡主类型 
        BYTE ICNB[4];             		//国际代码00000156
        BYTE provCode[2];         		//省级代码;
        BYTE cityCode[2];         		//城市代码
        BYTE unionCode[2];        		//互通卡种N0000-非互通表示只能本地使用	
        BYTE appStartDate[4];     		//YYYYMMDD
        BYTE appEndDate[4];       		//YYYYMMDD	
        BYTE cardType;			// 卡种类 
        BYTE cardIssueId[4];			//发卡机构标识
        BYTE lastCityCode[2];    		//进闸方城市代码-
        BYTE lastInstitCode[8];  		//进闸方机构标识-
        BYTE lastStation[2];     		//进闸站点 0
        BYTE lastTerm[8];        		//进闸终端编号  
        BYTE lastTim[7];         		//进闸时间
        //复合记录
        BYTE recordId[2];         //记录ID2701
        BYTE recordLen;           //0x7D
        BYTE appExpFlag;          //应用有效标识0x01
        BYTE tuionFlag;           //互联互通交易标识
        BYTE appLockFlag;         //应用锁定标志（0-应用没有锁定；1-应用锁定,应用程序判断）
        BYTE tranRef[8];          //交易流水号BCD
        BYTE status;              //00：初始值-禁止使用; 01：进站; 02：出站; 03：进站更新; 04：出站更新
        //进出闸数据
        BYTE enterCityCode[2];    //进闸方城市代码
        BYTE exitCityCode[2];     //出闸方城市代码
        BYTE enterInstitCode[8];  //进闸方机构标识
        BYTE exitInstitCode[8];   //出闸方机构标识-
        BYTE enterStation[8];     //进闸站点
        BYTE exitStation[8];      //出闸站点
        BYTE enterTerm[8];        //进闸终端编号
        BYTE exitTerm[8];         //出闸终端编号
        BYTE entertim[7];         //进闸时间YYYYMMDDhhmmss
        BYTE exitTim[7];          //出闸时间YYYYMMDDhhmmss
        BYTE fare[4];             //标记金额       
        BYTE enterLine;           //进闸线路    	
        BYTE exitLine;            //出闸线路		
        BYTE enterMoney[4];       //交易金额(高字节在前)-
        BYTE enterBalance[4];     //交易后余额(高字节在前)
        BYTE exitMoney[4];        //交易金额(高字节在前)-
        BYTE ref[29];             //预留
    }TUAnalyseOut, * pTUAnalyseOut;

    typedef struct
    {
        BYTE TradeType;//交易类型
        BYTE TicketMainTypeCode;//车票主类型编码
        BYTE TicketTypeCode;//车票类型编码
        BYTE TicketPyhType;//物理类型
        BYTE PhyID[8];//物理卡号
        BYTE LogicID[8];//逻辑卡号
        BYTE RFU;//
        BYTE OperatorCode[4];//操作员编号
        BYTE SamID[6];//SAM 卡号
        BYTE TradeTm[7];//交易时间
        BYTE ServiceDate[4];//运营日期
        BYTE PayType;//支付方式
        BYTE BeforeBalance[4];//交易前票值
        BYTE TradeCount[2];//交易次数
        BYTE TradeAmount[2];//交易金额
        BYTE CurSatationID[4];//当前车站
        BYTE LastSatationID[4];//上次使用车站
        BYTE DeviceID[4];//设备标识
        BYTE TerminalSerial[4];//终端交易流水号
        BYTE TicketCnt[2];//票卡计数
        BYTE CityCode[2];//城市代码
        BYTE PurseDate[4];//钱包起始日期
        BYTE Deposit[2];//押金
        BYTE Discount[2];//优惠金额
        BYTE OCTLogicID[8];//储值票逻辑卡号
        BYTE ExitTktFlag;//出站票交易标记
        BYTE MemoryTktFlag;//纪念卡交易标记
        BYTE TestTktFlag;//测试卡交易标记
        BYTE AccTradeTM[7];//ACC主机交易时间
        BYTE RFU1[4];//
        BYTE RFU2[8];//
        BYTE TAC[4];
    }MTRCARD_TRADE_INFO, * PMTRCARD_TRADE_INFO;

    typedef struct
    {
        BYTE TradeType;//交易类型
        BYTE TicketMainTypeCode;//车票主类型编码
        BYTE TicketTypeCode;//车票类型编码
        BYTE TradePyhType;//物理类型
        BYTE LogicID[8];//逻辑卡号
        BYTE Version;//版本号BYTE RFU;//
        BYTE OperatorCode[4];//操作员编号
        BYTE SamID[6];//SAM 卡号
        BYTE TradeTm[7];//交易时间    30
        BYTE PayType;//支付方式
        BYTE Prebalance[4];//交易前票值
        BYTE RFU[2];////BYTE TradeCount[2];//交易次数
        BYTE TradeAmount[2];//交易金额
        BYTE CurSatationID[4];//当前车站
        BYTE LastSatationID[4];//上次使用车站    47
        BYTE DeviceID[4];//设备标识
        BYTE TraeSerial[4];//终端交易流水号
        BYTE TicketCnt[2];//票卡计数
        BYTE CityCode[2];//城市代码
        BYTE CardMainType;//卡主类型
        BYTE CardSubType;//卡子类型
        BYTE CardAppType;//卡行业应用类型
        BYTE BusinessType;//卡业务类型        63
        BYTE Deposit[2];//押金
        BYTE Discount[2];//优惠金额
        BYTE SAMLogic[4];//SAM卡序列号
        BYTE SAMSerial[4];//SAM卡流水号
        BYTE DiscountType;//优惠类型
        BYTE ExchangeTm[7];//换乘最近时间        82
        BYTE ExchangeLastAmount[2];//换乘上笔交易金额
        BYTE ExchangeNearCode[6];//换乘最近单位编号
        BYTE ExchangeNearLineCode[4];//换乘最近线路编号
        BYTE RechargeTradeTM[7];//ACC交易时间    102
        BYTE RFU1[20];//
        BYTE TAC[4];
    }OCTCARD_TRADE_INFO, * POCTCARD_TRADE_INFO;

    typedef struct
    {
        BYTE tradeType;   //交易类型码
        BYTE ticketMainType;  //车票主类型码
        BYTE ticketType;  //车票类型码
        BYTE phyType;    //票卡物理类型-芯片类型70
        BYTE phyID[8];       //物理卡号
        BYTE logicID[20]; //逻辑卡号卡CSN, 长度不足时左补0
        BYTE cardVer;    //卡版本号
        BYTE OperatorCode[4];   //操作员编码long(M)仅用于BOM
        BYTE SAMID[6];       //SAMID
        BYTE dateTime[7]; //交易时间日期
        BYTE payMode;    //支付方式
        BYTE balanceOrg[4];   //交易前金额
        BYTE ref[2];     //预留
        BYTE transAmount[2];  //交易金额
        BYTE curStationSN[4]; //当前车站
        BYTE lastStationSN[4];    //上次车站
        BYTE deviceSN[4]; //终端设备标识-上位机设备编号
        BYTE seq[4];     //终端交易流水号
        BYTE ticketCounter[2];    //票计数器
        BYTE cityCode[2]; //城市代码
        BYTE cardMainType;    //卡主类型
        BYTE cardSubType; //卡子类型
        BYTE appType;    //行业应用类型   00：消费正常交易09：消费灰交易
        BYTE tt;     //业务类型 06：电子钱包消费09：复合消费
        BYTE deposit[2];  //押金仅用于出售储值票交易
        BYTE disCountAmount[2]; //优惠金额--全价票额与实际交易额的差值
        BYTE samSerial[8];    //SAM卡序列号
        BYTE samSeq[4];      //SAM卡流水号
        BYTE discountType;    //优惠类型 0-无优惠1-换乘优惠2-时段优惠
        BYTE lastExchangeTm[7];   //换乘最近时间   YYYYMMDDHHMMSS
        BYTE lastExchangeAmt[2];//换乘时上一笔交易金额
        BYTE lastExchangeID[6];//换乘最近单位编号
        BYTE lastExchangeLine[4];//换乘最近线路编号
        BYTE accTime[7];  //ACC主机交易时间    用于联机充值
        BYTE purseType;//0x00 电子钱包， 0x01 电子现金
        BYTE issueInstruct[4];
        BYTE expiredDate[4];
        BYTE tuCardType;//互联互通卡种类型
        BYTE unpredictNumber[4];
        BYTE enterInstructCode[18];
        BYTE ref2[20];
        BYTE TAC[4]; //TAC
    }TUCARD_TRADE_INFO, * PTUCARD_TRADE_INFO;

    typedef struct
    {
        BYTE ICType;//芯片类型
        BYTE TraderRespData[512];

    }UPDATE_RESP, * PUPDATE_RESP;

    typedef struct
    {
        BYTE TradeAmount[4];
        BYTE Balance[4];
        MTRCARD_TRADE_INFO MTRTradeData[113];
    }MTR_UPDATE_RESP, * PMTR_UPDATE_RESP;

    typedef struct
    {
        BYTE TradeAmount[4];
        BYTE Balance[4];
        OCTCARD_TRADE_INFO OCTTradeData[128];
    }OCT_UPDATE_RESP, * POCT_UPDATE_RESP;

    typedef struct
    {
        BYTE TradeAmount[4];
        BYTE Balance[4];
        TUCARD_TRADE_INFO TUTradeData[182];
    }TU_UPDATE_RESP, * PTU_UPDATE_RESP;


    /**
     * @brief 读写器接口库日志打印开关
     * @param is 传入 true 或 false 表示开或关
     */
    DLL_EXPORT void loggingSwitch(bool is);

    /**
     * @brief 打开读写器与设备连接的串口
     * @param portsSerial 串口编号
     */
    DLL_EXPORT BYTE readerComOpen(int portsSerial);

    /**
     * @brief 关闭读写器串口
     */
    DLL_EXPORT BYTE readerComClose();

    /**
     * @brief 读写器初始化/自检
     * @param deviceID 设备编号
     */
    DLL_EXPORT BYTE readerInit(BYTE* deviceID, BYTE& Producter);

    /**
     * @brief 获取读写器版本
     * @return PVERSION_INFO 读写器版本信息结构体
     */
    DLL_EXPORT BYTE getVersion(PVERSION_INFO ReaderVersion);

    /**
     * @brief 获取读写器状态及SAM卡号
     * @return PSTATUS_INFO 读写器状态信息结构体
     */
    DLL_EXPORT BYTE getStatus(PSTATUS_INFO ReaderStatus);

    /**
     * @brief 获取读写器参数版本
     * @return PARAM_VERSION 参数版本信息结构体
     */
    DLL_EXPORT BYTE getParamVersion(PPARAM_VERSION ParamVersion);

    /**
     * @brief 调用读写器进行寻卡
     * @param Anti 天线编号，0为天线1，1为天线2
     * @return PSEARCHCARD_INFO 寻卡信息结构体
     */
    DLL_EXPORT BYTE cardSearch(BYTE Anti, PSEARCHCARD_INFO SearchCardInfo);

    /**
     * @brief 调用读写器执行票卡分析操作
     * @param Anti 天线编号，0为天线1，1为天线2
     * @param AnalyseZone 分析区域，0为非付费区，1为付费区
     * @return PANALYSECARD_RESP 卡片分析结果信息结构体
     */
    DLL_EXPORT BYTE cardAnalyse(BYTE Anti, BYTE AnalyseZone, PANALYSECARD_RESP pAnalyseInfo);

    /**
     * @brief 调用读写器执行票卡更新操作
     * @param Anti 天线编号，0为天线1，1为天线2
     * @param UpdateInfo 票卡更新信息传入，具体参考PUPDATE_CARD_IN结构体
     * @return UPDATE_RESP 票卡更新返回信息结构体
     */
    DLL_EXPORT BYTE cardUpdate(BYTE Anti, PUPDATE_CARD_IN UpdateInfo, PUPDATE_RESP pUpdateTradeData);

    /**
     * @brief 调用读写器执行token票退票操作
     * @param Anti 天线编号，0为天线1，1为天线2
     * @param pRefundInfo 票卡退票信息传入，具体参考PUL_REFUND_IN结构体
     * @return PMTRCARD_TRADE_INFO token退票返回交易数据信息结构体
     */
    DLL_EXPORT BYTE cardRefund(BYTE Anti, PUL_REFUND_IN pRefundInfo, PMTRCARD_TRADE_INFO pRefundTrade);

    /**
     * @brief 调用读写器读取票卡历史交易
     * @param Anti 天线编号，0为天线1，1为天线2
     * @return PHISTORY_RESP 票卡历史交易记录信息结构体
     */
    DLL_EXPORT BYTE readCardHistory(BYTE Anti, PHISTORY_RESP pHistoryOut);

    /**
     * @brief 调用读写器计算费用
     * @param pFareInfo 费用计算信息传入，具体参考PFARECAL_IN结构体
     * @return PFARECAL_OUT 费用计算结果信息结构体
     */
    DLL_EXPORT BYTE calcFare(PFARECAL_IN pFareInfo, PFARECAL_OUT pFareResp);

    /**
     * @brief 读写器软件更新与参数下载
     * @param FilePath 下载进入读写器的文件路径
     * @param FileName 下载进入读写器的文件名称
     */
     //DLL_EXPORT BYTE fileDownload(CString FilePath, CString FileName);




#ifdef __cplusplus
}
#endif 





