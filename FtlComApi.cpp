// FtlComApi.cpp : Defines the exported functions for the DLL.
// property: Common Language Runtime Support: Common Language Runtime Support (/clr)  ==> for c#
// use DUMPBIN to examine COFF object files, standard libraries of COFF objects, executable files, and dynamic-link libraries (DLLs).
// commamd line : DUMPBIN [options] files... ==>   DUMPBIN  /OUT  c:\ aa.txt
// 在Project的 Property頁面,C/C++, Language,Conformance mode : No
// Use DLL Export Viewer


#include "pch.h"
#include "framework.h"
#include "FtlComApi.h"


int  EDCErrorCode;
int  iEDCTscDataLen;
char cEDCHostResponseCode[20];
char cEDCTscData[MAXPACKQRYDATALEN];
char cEDCErrMsg[1024];

// This is an example of an exported variable
FTLCOMAPI_API int nFtlComApi=0;

// This is an example of an exported function.
FTLCOMAPI_API int __stdcall fnFtlComApi(void)
{
    EDCErrorCode = 0;
    iEDCTscDataLen = 0;
    memset(cEDCHostResponseCode, 0, sizeof(cEDCHostResponseCode));
    memset(cEDCTscData, 0, sizeof(cEDCTscData));
    memset(cEDCErrMsg, 0, sizeof(cEDCErrMsg));

    return 99;
}


FTLCOMAPI_API int __stdcall FtlComApiInit(void)
{
    string stmp;
    StCliEDC sEdcSet;

    stmp = "TsccThread";
    CIniReader Clini;

    if (!_DirExists("c:\\Ftlinline\\dll\\dat\\"))
        SHCreateDirectory(NULL, L"c:\\Ftlinline\\dll\\dat\\");

    try
    {
        //READ TsccThread INI
        int iDebugSw=0;

        Clini.setINIFileName(INLINEINIFILE);

        sEdcSet.TscComDeviceName = Clini.ReadString("TsccThread", "TscComDeviceName", "COM0");
        sEdcSet.TscComBaudRate = stoi(Clini.ReadString("TsccThread", "TscComBaudRate", "9600")); // .c_str() );
        sEdcSet.TscComParity = stoi(Clini.ReadString("TsccThread", "TscComParity", "0"));  //0:N, 
        sEdcSet.TscComByteSiz = stoi(Clini.ReadString("TsccThread", "TscComByteSiz", "8"));
        sEdcSet.TscComStopBits = stoi(Clini.ReadString("TsccThread", "TscComStopBits", "1"));

        sEdcSet.TsccEnqIntervalSec = stoi(Clini.ReadString("TsccThread", "TsccEnqIntervalSec", "4000"));      // X31 TimeOut
        sEdcSet.TscComReceiveInterval = stoi(Clini.ReadString("TsccThread", "TscComReceiveInterval", "3"));//TscComReceiveInterval: 每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2)) 之偵測 Interval 3ms
        sEdcSet.TscComReqTimeOut = stoi(Clini.ReadString("TsccThread", "TscComReqTimeOut", "30000"));      //TsccEnqIntervalSec: 每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms
        sEdcSet.TsccReqIntervalCnt = stoi(Clini.ReadString("TsccThread", "TsccReqIntervalCnt", "3"));      //????
        sEdcSet.TsccReqIntervalTime = stoi(Clini.ReadString("TsccThread", "TsccReqIntervalTime", "1"));    //????
        sEdcSet.TsccRtyConnectCnt = stoi(Clini.ReadString("TsccThread", "TsccRtyConnectCnt", "3"));        //連線重試次數
        sEdcSet.TsccRtyConnectIntervalTime = stoi(Clini.ReadString("TsccThread", "TsccRtyConnectIntervalTime", "1000"));  //????
        
        //iTsccReqTimeOut = stoi(Clini.ReadString("TsccThread", "TsccReqTimeOut", "35000"));
        //iTscX42TimeOut = stoi(Clini.ReadString("TsccThread", "TscX42TimeOut", "35000")); ;
        iDebugSw = stoi(Clini.ReadString("TsccThread", "TsccDebugSw", "0"));
       
        //READ END
        //WRITE X INI
        Clini.WriteString("TsccThread", "TscComDeviceName", sEdcSet.TscComDeviceName);
        Clini.WriteString("TsccThread", "TscComBaudRate", _int2str(sEdcSet.TscComBaudRate));
        Clini.WriteString("TsccThread", "TscComParity", _int2str(sEdcSet.TscComParity));
        Clini.WriteString("TsccThread", "TscComByteSiz", _int2str(sEdcSet.TscComByteSiz));

        Clini.WriteString("TsccThread", "TTscComStopBits", _int2str(sEdcSet.TscComStopBits));
        Clini.WriteString("TsccThread", "TsccEnqIntervalSec", _int2str(sEdcSet.TsccEnqIntervalSec));
        Clini.WriteString("TsccThread", "TscComReceiveInterval", _int2str(sEdcSet.TscComReceiveInterval));
        Clini.WriteString("TsccThread", "TscComReqTimeOut", _int2str(sEdcSet.TscComReqTimeOut));
        Clini.WriteString("TsccThread", "TsccReqIntervalCnt", _int2str(sEdcSet.TsccReqIntervalCnt));

        Clini.WriteString("TsccThread", "TsccReqIntervalTime", _int2str(sEdcSet.TsccReqIntervalTime));
        Clini.WriteString("TsccThread", "TsccRtyConnectCnt", _int2str(sEdcSet.TsccRtyConnectCnt));
        Clini.WriteString("TsccThread", "TsccRtyConnectIntervalTime", _int2str(sEdcSet.TsccRtyConnectIntervalTime));
        Clini.WriteString("TsccThread", "TsccDebugSw", _int2str(iDebugSw));

        //logsprintf("GetClint(%s) OK", sSection.c_str());

        //WRITE END
    }
    catch (...)
    {
        ;; //logsprintf("GetClint(%s) Error...", sSection.c_str());
    }

    return(0);
}


FTLCOMAPI_API int __stdcall FtlComApiTest(int TranType, int TMode, byte* InData, byte* OutData, int iAmt, byte* Cmd)
{
    int result;
    
    cLsEDC* CommEDC = new cLsEDC();
    result = CommEDC->EDCApiInit(0,0, "0123456789");  
    //result = CommEDC->EDCApiComTest(0, 0, "0123456789");

    sprintf_s(cEDCHostResponseCode, "0000");
    sprintf_s(cEDCTscData, "0000");
    sprintf_s(cEDCErrMsg, "REQUEST_TEST_OK");
    iEDCTscDataLen = 4; //0000F
   
    delete CommEDC;

    return result;
}



FTLCOMAPI_API int __stdcall FtlComApiGetRep(int TranType, int TMode, byte* RepData, byte* OutData, int &OutDataLen)
{
       
    memset(RepData, 0, sizeof(RepData));
    memcpy(RepData, cEDCHostResponseCode, sizeof(cEDCHostResponseCode));
    
    OutDataLen = iEDCTscDataLen;
    memset(OutData, 0, sizeof(OutData));
    memcpy(OutData, cEDCTscData, iEDCTscDataLen);

    EDCErrorCode = 0;
    iEDCTscDataLen = 0;
    memset(cEDCHostResponseCode, 0, sizeof(cEDCHostResponseCode));
    memset(cEDCTscData, 0, sizeof(cEDCTscData));
    memset(cEDCErrMsg, 0, sizeof(cEDCErrMsg));

    return(0);
}

//xdata->WriteData(string((char*)StoreNO), string((char*)EcrNO), string((char*)SalesNo), string((char*)SalFileName), string((char*)Version));

FTLCOMAPI_API int __stdcall FtlComApi(int TranType, int TMode, byte * InData, byte * OutData, int iAmt, byte * Cmd)
{
    int result;

    cLsEDC* CommEDC = new cLsEDC();    
    //result = CommEDC->EDCApiComTest(0, 0, "0123456789");
    
    //if( TranType>699 && TranType<799 )    //e通卡 FETC ComPor IO
    switch (TranType) 
    {
     //int __fastcall cLsEDC::EdcApiCall(int TranType, int TMode, byte * lpbInData, byte * lpbOutData, int iAmt, byte * Cmd)
     case 0: 
         result = CommEDC->EDCApiInit(0, 0, "0123456789");
         sprintf_s(cEDCHostResponseCode, "0000");
         sprintf_s(cEDCTscData, "0000");
         sprintf_s(cEDCErrMsg, "REQUEST_TEST_OK");
         iEDCTscDataLen = 4; //0000

         break;

         /*
     //case 700:  // FETC 查詢
     //case 706:  //異常交易補登 0x60/0x61/0x40/0x50   2010/10/13 Update
     //   iRtn = Fun_HighWayEDC_A080(TranType, AddValue);
     //   break;
     //case 701:  // FETC 加值
     //   iRtn = Fun_HighWayEDC_A140(TranType, AddValue);
     //   break;
     //case 702:  // FETC 加值取消
     //   iRtn = Fun_HighWayEDC_A120(TranType, AddValue);
     //   break;
         */

    //case 703:  // "FE01 信用卡卡片查詢" , "FE00信用卡交易終止"
    //    iRtn = Fun_HighWayEDC_FE01(TranType, AddValue);
    //    break;
    case 704:  //信用卡交易 0010:信用卡, 0011:紅利 ,8010:信用卡, 2010:銀聯, 3010:SmartPay
    case 705:  //信用卡退貨 0030, 0031
    case 707:   // "FE01, FE02 FE03, FE04  SmartPay卡片查詢"     ; FE04 Lusung Add 2018/06/01
    case 708:  // FETC 存在
    case 709:  // FETC 結帳
    case 710:  //電簽啟動訊息
    case 711:  //電簽確認訊息
    {
        result = CommEDC->EdcApiCall(TranType, TMode, InData, OutData, iAmt, Cmd);
        
        EDCErrorCode = result;
        iEDCTscDataLen = CommEDC->iTscDataLen;
        memcpy(cEDCHostResponseCode, CommEDC->ucHostResponseCode, sizeof(CommEDC->ucHostResponseCode));
        memcpy(cEDCTscData, CommEDC->ucTscData, CommEDC->iTscDataLen);
        memcpy(cEDCErrMsg, CommEDC->sTsccApiErrorMsg.c_str(), CommEDC->sTsccApiErrorMsg.length());
        break;
    }
 
    default:
        result = REQUEST_NOT_OK;
        sprintf_s(cEDCHostResponseCode, "60A1");
        sprintf_s(cEDCTscData, "60A1");
        sprintf_s(cEDCErrMsg, "REQUEST_NOT_OK");
        iEDCTscDataLen = 4; //60A1
    }
   

    delete CommEDC;

    return result;

}


// This is the constructor of a class that has been exported.
CFtlComApi::CFtlComApi()
{
    return;
}



