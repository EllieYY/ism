#pragma once


#define WINDOWS
#define EXPORT

#include <malloc.h>
#ifdef WINDOWS
#ifdef EXPORT
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif
#else
#include <stdlib.h>

#ifdef EXPORT
#define EXPORT_API __attribute__((visibility ("default")))
#else
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif 
	
#ifndef EXPORT_API
#define EXPORT_API
#endif
	
	///
	/// @brief ��������Ҫ��¶�Ľӿں���
	///
	//EXPORT_API double seekArea(int r, int h);
//20211110ֽ��ʶ����	
	EXPORT_API int BIM_Connect(int nPortNo);
	EXPORT_API int BIM_DisConnect();
	EXPORT_API int BIM_Poll();
	EXPORT_API int BIM_Reset();
	EXPORT_API int BIM_Initial();
	EXPORT_API int BIM_OPEN();
	EXPORT_API int BIM_CLOSE();
	EXPORT_API int BIM_RequestStatus(char *Model,unsigned char *Number,char *ErrStatus,char *Ainfo,char *BankNote);
	EXPORT_API int BIM_IntoEscrow(unsigned char *Number);
	EXPORT_API int BIM_IntoCashBobx();
	EXPORT_API int BIM_IntoCashBobx();
	EXPORT_API int BIM_ReturnTransport(void);
	EXPORT_API int BIM_ReturnBankNote(void);

//��Ӳ��ʶ���˱����Ĺ���Ҳ�ӵ�һ��
	EXPORT_API int BRC_Connect(int nPortNo);
	EXPORT_API int BRC_Close();
	EXPORT_API int SimplePoll();
	EXPORT_API int BRC_Reset();
	EXPORT_API int Perform_Self_Check();
	EXPORT_API int Request_Software_Vevsion(char *Ver);
	EXPORT_API int Modify_inhibit_Open();
	EXPORT_API int Modify_inhibit_Closes();
	EXPORT_API int Clear_Money_Counters();
	EXPORT_API int Request_money_in();
	EXPORT_API int Request_Hopper_Balance();
	EXPORT_API int Dispense_Hopper_Pattern(int Count);
//	EXPORT_API int Modify_Hopper_Balance(int Count);
//	EXPORT_API int Request_Cashbox_Counter();
	EXPORT_API int Clear_Cashbox_Counter();
	EXPORT_API int Dispense_Hopper_All();
//---------------------------------------------
	EXPORT_API int F53Bill_Connect(int nPortNo);
	EXPORT_API int F53Bill_DisConnect();
	EXPORT_API int F53Poll();
	EXPORT_API int F53Bill_Setting(unsigned char Cst1,unsigned char Cst2);
	EXPORT_API int F53Bill_BillCount(unsigned char Count1,unsigned char Count2);
	EXPORT_API int F53Bill_Cancel(void);

//���η�װ
	EXPORT_API int ConnectMachine(int BillPortNo,int CoinPortNo,int ChangePortNo);
	EXPORT_API int Modify_Hopper_Balance(int Count);
	EXPORT_API int StartPutCoin(int TotalMoney);
	EXPORT_API int CheckCoinStatus(int *BankNoteCount,int *CoinCount);
	EXPORT_API int StopPutCoin(int *BillMoney,int *CoinMoney);
	EXPORT_API int Request_Cashbox_Counter();
	EXPORT_API int ChanceCoin(int ChangeCount, int *BillMoney,int *CoinMoney);
	EXPORT_API int ResultOperate(int InOutFlag);
	EXPORT_API int CloseAllCom(void);

#ifdef __cplusplus
}
#endif 


