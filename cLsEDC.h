#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <atlstr.h> 
#include <tchar.h>
#include <string>
#include <sstream> 
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;


#include "SerialPort.h"
#include "TsccDongleApi.h"
#include "INI.h"
#include "cLsCommon.h"
#include "Error.h"

using namespace std;
//using namespace System;

/**
#define SUCCEED         0
#define FAILURE        -1
#define EDCTIMEOUT     -99

#define ESC 0x1B
#define STX     0x02
#define ETX     0x03
#define ACK     0x06
#define NAK     0x15
#define CR      0x0D
#define LF      0x0A
**/

#define MAX_PACK_DATA_SIZE  10240
#define DLLRCVBUFFER        102400
#define MAXPACKQRYDATALEN   4096  //Max Pack Data  Dont Over 4096


#define EDCSIGFILE        "c:\\Ftlinline\\dll\\dat\\EdcSig.gz"
#define EDCSIGFILETMP     "c:\\Ftlinline\\dll\\dat\\EdcSig.tmp"
#define TRANSEDCSIGFILE   "c:\\Ftlinline\\dll\\dat\\TransEdcSig.gz"
#define INLINEINIFILE     "c:\\Ftlinline\\dll\\ClientInLine.ini"

#define BytesAvailable    "BytesWaiting"

typedef struct {
    string TscComDeviceName;
    int    iDeviceNo;
    int    TscComBaudRate;
    int    TscComByteSiz;
    int    TscComParity;
    int    TscComStopBits;

    int    TsccEnqIntervalSec; 
    int    TsccReqIntervalTime; 
    int    TsccReqIntervalCnt;  
    int    TsccRtyConnectIntervalTime; 
    int    TsccRtyConnectCnt;  
    int    TscComReceiveInterval;  
    int    TscComReqTimeOut;   

} StCliEDC;

class cLsEDC
{
   private:
       bool ComConnected;
       int iTscIntervalTime;
       int iTscIntervalSec;    //設定 如果 已處於Qry時, 等待多少秒

       int iChkLrc;            //  1:是 0:否要檢查 Lrc
       int iReqIntervalTime;   // Tcp 封包接收等待 ,採用時間 mSec, 優先使用
       int iReqIntervalCnt;    // Tcp 封包接收等待 ,採用次數

       int iRtyConnectIntervalTime;   // 連線重試等待時間  mSec
       //int iRtyConnectCnt;            // 連線重試次數
       int iTscErrorCode;
       int iCommitStatus;
       int iTsccReqTimeOut;
       int iTscX42TimeOut;            // default 35000 Sec
       int iTscX40TimeOut;            // default 40000 Sec
       int iTscX43ConfirmTimeout;     //default 5000
       int  TErrCode;

       string sReceivedData;
       string sTenCode;
       string sEcrNo;
       string sTranNo;
       string sSalesNo;
       string sInvoNo;
       string sTranCode;
       string sTimeOut;
       string sBandCode;
       string sCard_no;


       // int iForceDatDll;
       // int TsccOffLineApi;
       
       CIniReader Clini;
       StCliEDC sEdcSet;
       CSerialPort SCOMx;
       DCB OriginalDCB;
       DCB MyDCB;
       COMMTIMEOUTS MyTimeouts;
       COMMTIMEOUTS OriginalTimeouts;


       string ComNo;

       int __fastcall  GetClint(string sSection);
       void __fastcall EdcTraceLog(string sLog);
       void __fastcall RtnLogMsg(string sLog);
       void __fastcall EdcTraceLog(LPSTR pstr);
       void __fastcall logsprintf(const char* format, ...);
       void __fastcall Debuglogsprintf(const char* format, ...);

       string  __fastcall LRC_VALUE(string AStr); //2 bytes
       string  __fastcall LRC_VALUE(LPSTR ptr, int iLen);
       int __fastcall BcdToInt(LPSTR pBcdData2);
       void __fastcall IntToBcd(int iBcd, LPSTR pBcdData2);


       int __fastcall InitComPort();
       int __fastcall CloseComPort();
       int __fastcall EDC_CommR(int iTranType, LPSTR pWDongleData, int* piWDongleDataLength, int ComTimeOut);
             
       int __fastcall HighWayEDCComm(LPSTR sPtr, int sPtrLen, LPSTR rPtr, int *rPtrLen, int iTimeOut, bool RepACK=TRUE);
       void _fastcall SetHostResponseCode(int iRtn);   //將 Rep Code or Error Code save to LsEDC

       int __fastcall HighWayEDC_Sig(int iTranType, LPSTR ptr, int iCash);
       int __fastcall HighWayEDC_41_Sig(LPSTR ptr, int iCash);
       int __fastcall RepEDC_42(LPSTR ptr, int iLen);
       int __fastcall HighWayEDC_42Data(int ComTimeOut, int WaitSec42TimeOut);
       int __fastcall HighWayEDC_42_ToFile(LPSTR ptr, int iDataLen, LPSTR pRepDownLoadInfo, int& iRe_SendCount, int& iRecCount);
       int __fastcall HighWayEDC_43_Confirm(LPSTR ptr, int iCash);

protected:
    

     //static property int aa;

public:
   /*
     property int TranMode {
        int get() { return TranModeSw; }
        void set(int value) { TranModeSw = value; }
    }
    */

    //property int CHKLRC = { read = iChkLrc, write = iChkLrc, default = 0 };
    bool DebugSw;

    int TraceLog;
    int TranModeSw;
    int EDCTranType;
   
    char ucHostResponseCode[20];
    char ucTscData[MAXPACKQRYDATALEN];
    int  iTscDataLen;
    string sOutputData;
    string sTsccApiErrorMsg;

    CSerialPort  LsEDC;
    int  __fastcall EDCApiInit(int iTranType, int iAddValue, string ApiInputData);
    int  __fastcall EDCApiComTest(int iTranType, int iAddValue, string ApiInputData);

    int __fastcall EdcApiCall(int TranType, int TMode, byte* lpbInData, byte* lpbOutData, int iAmt, byte* Cmd);

    int __fastcall Fun_HighWayEDC_Cancel(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData);
    int  __fastcall Fun_HighWayEDC_0x32_0x40(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData);
    //int __fastcall Fun_HighWayEDC_FE01(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData); 2020/11/03 確認不再使用
    int __fastcall Fun_HighWayEDC_FE02(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData);
    int  __fastcall HighWayEDC_Exists(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData);
    int  __fastcall Fun_HighWayEDC_A090(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData);  //結帳
    int  __fastcall Fun_HighWayEDC_Sig(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData);
    int  __fastcall Fun_HighWayEDC_Confirm(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData);
   

    cLsEDC()
    {

        if (!_DirExists("c:\\Ftlinline\\dll\\dat\\"))   
            SHCreateDirectory(NULL, L"c:\\Ftlinline\\dll\\dat\\");    

        if (!_DirExists("c:\\Ftlinline\\log\\"))
            SHCreateDirectory(NULL,L"c:\\Ftlinline\\log\\");
       
        TraceLog=0;
        TranModeSw=0;
        iTscIntervalTime=3;
        iTscIntervalSec=3;    //設定 如果 已處於Qry時, 等待多少秒
        iChkLrc=1;            //  1:是 0:否要檢查 Lrc
        iReqIntervalTime=1;   // Tcp 封包接收等待 ,採用時間 mSec, 優先使用
        iReqIntervalCnt=3;    // Tcp 封包接收等待 ,採用次數

        iRtyConnectIntervalTime=100;   // 連線重試等待時間  mSec
        //iRtyConnectCnt=3;            // 連線重試次數
        iTscErrorCode=0;
        iCommitStatus=0;
        iTsccReqTimeOut=35000;
        iTscX42TimeOut=35000;            // default 35 Sec
        iTscX40TimeOut = 35000;          // default 35 Sec
        iTscX43ConfirmTimeout = 5000;    // default 5000; 

        DebugSw = true;               // false;
        ComConnected = false;
        TErrCode = 0;
        sReceivedData = "";
        sTsccApiErrorMsg = "";

        memset(ucHostResponseCode, 0, sizeof(ucHostResponseCode));

        sTenCode = "";
        sEcrNo = "";
        sTranNo = "";
        sSalesNo = "";
        sInvoNo = "";
        sTranCode = "";
        sTimeOut = "";
        sBandCode = "";
        sCard_no = "";


        iTscDataLen = 0;
        EDCTranType = 0;
        memset(ucTscData, 0, sizeof(ucTscData));
        sOutputData="";

        Clini.setINIFileName(INLINEINIFILE);
       

    }

    ~cLsEDC()
    {
        //Close();
    }
   // ~cLsEDC();
   //  cLsEDC();


};

//extern REPORT *report;

/***

 [TsccThread]
TsccSvrIp=172.16.11.20
TsccPort=8108
KsmSvrIp=172.16.24.1
KsmSvrIpPoint=172.16.24.2
KsmAddPort=8787
KsmPotPort=8788
TsccThreadDebug=1
TsccThreadChkLrc=1
TsccEnqIntervalSec=5
TsccReqIntervalTime=0
TsccReqIntervalCnt=10000
TsccRtyConnectIntervalTime=1000
TsccRtyConnectCnt=1
TsccOffLineApi=0
TscComDeviceName=COM6
TscComReceiveInterval=11
TscComBaudRate=115200
TscComByteSize=8
TscComParity=0
TscComStopBits=1
TscComReqTimeOut=16000
TscTranModeSw=0
KsmTranModeSw=0
TscFetcComDeviceName=COM7
TwscTranModeSw=0
TWSCComPortNo=5
MutiDongleType=1

int __stdcall wx_ok(string StoreNO, string EcrNO, string SalesNo, string SalFileName, string Version)
{

if (!DirExists("C:\\FTLPOS\\XZDATA\\WX\\"))  //if (!DirectoryExists(str_x_path))
SHCreateDirectory(NULL, L"C:\\FTLPOS\\XZDATA\\WX\\");   //ForceDirectories(str_x_path);

DeleteFile(StringToWString(str_tmp_ini).c_str());
writelog("wx_ok 完成: 刪除檔案 " + str_tmp_ini);

return 1;


}
**/