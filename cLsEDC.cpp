#include "pch.h"
#include "cLsEDC.h"

using namespace std;


int  __fastcall cLsEDC::EDCApiInit(int iTranType, int iAddValue, string ApiInputData)
{

    ComConnected = false;
    TErrCode = 0;
    sReceivedData = "";
    sTsccApiErrorMsg = "";
   
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
    EDCTranType = iTranType;

    memset(ucHostResponseCode, 0, sizeof(ucHostResponseCode));
    memset(ucTscData, 0, sizeof(ucTscData));
    sOutputData = "";
        
    Debuglogsprintf("EDCApiInit: ApiInputData=%s", ApiInputData.c_str());

    //char bb[3];
    //IntToBcd(1, bb);
    //logsprintf("EDCApiInit: %d", 1);

    return(0);
}


int  __fastcall cLsEDC::EDCApiComTest(int iTranType, int iAddValue, string ApiInputData)
{
    int iRtn, i;

    logsprintf("EDCApiComTest: InitComPort  TranType=%d, AddValue=%d, ApiInputData=%s", iTranType, iAddValue, ApiInputData.c_str() );

    iRtn=InitComPort();
    logsprintf("EDCApiComTest: InitComPort  %s => %s", sEdcSet.TscComDeviceName.c_str(), iRtn == 0 ? "SUCCEED":"FAILE");

    Sleep(2000);

    i = CloseComPort();
    logsprintf("EDCApiComTest: CloseComPort %s => %s", sEdcSet.TscComDeviceName.c_str(), i == 0 ? "SUCCEED" : "FAILE");

    return(iRtn);

}


//
//HighWayEDCComm: 通訊 正常流程 
//sPtr: Input Data         去掉 STX(1), ETX(1) + LRC(2)
//sPtrLen : Input Data len 去掉 STX(1), ETX(1) + LRC(2)
//rPtr: Response Data      去掉 STX(1), ETX(1) + LRC(2)
//rPtrLen:  Response Data Len 去掉 STX(1), ETX(1) + LRC(2)
//return: SUCCEED 

int __fastcall cLsEDC::HighWayEDCComm(LPSTR sPtr, int sPtrLen, LPSTR rPtr, int *rPtrLen ,int iTimeOut, bool RepACK)
{
    int i, k, iRtn, rt1, CommTimeout, SrDataLen, iStartChar, iEndChar, recv_len;
    string s;  //send_data,tmp_cardno,recv_data,
    char SrData[1024], SendData[1024], LRC[3], RcvLrc[3], ACKNAKData[3];
   // HWEDC_Data_0x31   tmpData_0x31;

    iRtn = recv_len = iEndChar = 0;
    CommTimeout = iTimeOut; 
    if (EDCTranType == 708 || EDCTranType == 709  || iTimeOut >= 30)  //EDCTranType == 708 : FETC 存在 
        rt1 = 1;
    else
        rt1 = sEdcSet.TsccRtyConnectCnt; //3    連線重試次數

    *rPtrLen = 0;
    memset(rPtr, 0, sizeof(rPtr));
    memset(SrData, 0, sizeof(SrData));
    memset(SendData, 0, sizeof(SendData));
   
    memcpy(LRC, LRC_VALUE(sPtr, sPtrLen).c_str(), 2);
    LRC[2] = 0;
    SendData[0] = STX;
    memcpy(&SendData[1], sPtr, sPtrLen);
    SendData[1+sPtrLen] = ETX;
    memcpy(&SendData[1 + sPtrLen + 1], LRC, 2);
    SendData[1 + sPtrLen + 1 +2] = 0;
    SrDataLen = sPtrLen+4;

    for (i = 1; i <= rt1; i++)   //rt1 = ReTry 次數
    {
        try
        {
            logsprintf("HighWayEDCComm():Retry(%ld) Write->(%s) Len=%d, TimeOut=%d", i, logStr2Hex(SendData, SrDataLen).c_str(), SrDataLen, CommTimeout);
            SCOMx.Write(SendData, SrDataLen);    //Send data to EDC 
            
            //logsprintf("HighWayEDCComm():<-(Start Receive ..)");
            recv_len = 0;
            iRtn = EDCTIMEOUT;
            memset(SrData, 0, sizeof(SrData));
            recv_len = EDC_CommR(0, SrData, &k, CommTimeout);
            if (recv_len ==0)
            {
                iRtn= DATA_ERROR;     //"讀寫器錯誤!!"
                logsprintf("HighWayEDCComm(): Read Data Error TimeOut(%d)..收信失敗 <-(NULL)", CommTimeout);
                continue;
                               
            }
            else if (SrData[0] == NAK)
            {
                iRtn = REQUEST_NOT_OK;      
                logsprintf("HighWayEDCComm:<-(Receive NAK)(%s)", logStr2Hex(SrData, recv_len).c_str());
                //RtnLogMsg(s);
                memset(SrData, 0, sizeof(SrData));
                memset(rPtr, 0, sizeof(rPtr));
                Sleep(100);
                continue;
            }
            else
            {
                //memcpy(rPtr, SrData, len);
                //iStatus = _BcdToInt(&SrData[2]);
                iStartChar = iEndChar = 0;
                logsprintf("HighWayEDCComm():<-(End Reecive)(%s)", logStr2Hex(SrData, recv_len).c_str());
                // STX(1) + DATA[] + ETX(1) + LRC(2)  ==>去掉 STX(1), ETX(1) + LRC(2)
                for (k = 0; k < recv_len; k++)
                {
                    if (SrData[k] == STX || SrData[k] == ETX)
                    {
                        if(SrData[k] == STX)
                        {
                            iStartChar = k;
                        }
                        else  if (SrData[k] == ETX)
                        {
                            iEndChar = k;
                            *rPtrLen = (iEndChar - iStartChar) - 1;            // recv_len - 4;
                            memcpy(rPtr, &SrData[iStartChar + 1], *rPtrLen);   // recv_len - 4);    ///?? Lost Data
                            Debuglogsprintf("HighWayEDCComm():DATA(%s)", logStr2Hex(rPtr, *rPtrLen).c_str());
                            iRtn = SUCCEED;
                            break;
                        }
                       
                    }
                }
               
            }


            if (iEndChar != 0)  // Get ETX Char
            {
                //memcpy(RcvLrc ,&SrData[len-2], 2);
                memcpy(RcvLrc, &SrData[iEndChar + 1], 2);
                RcvLrc[2] = 0;
                s = LRC_VALUE(rPtr, *rPtrLen);
                if (_stringToUpper(s) != string(RcvLrc))
                {
                    //若資料發生 CHECKSUM ERROR 即 LRC Error) Error)，則送出 NAK ，至少重送二次。
                    iRtn = DATA_ERROR;
                    logsprintf("HighWayEDCComm:<-(接收)RevLrc(%s) != LRC(%s) ...Error ", RcvLrc, s.c_str());
                    //RtnLogMsg(s);
                    memset(SrData, 0, sizeof(SrData));
                    memset(rPtr, 0, sizeof(rPtr));
                    ACKNAKData[0] = NAK; ACKNAKData[1] = 0;
                    SCOMx.Write(ACKNAKData, 1);    //Send NAK to EDC 
                    Sleep(50);
                    continue;
                }
                else
                {
                    if (RepACK)
                    {
                        ACKNAKData[0] = ACK; ACKNAKData[1] = 0;
                        SCOMx.Write(ACKNAKData, 1);    //Send ACK to EDC 
                        Debuglogsprintf("HighWayEDCComm: (Receive 完成..Send ACK to EDC)");
                    }
                    else
                    {
                        Debuglogsprintf("HighWayEDCComm: (Receive 完成)");
                    }
                    
                    iRtn = SUCCEED;
                    break;
                }
            }
            else
            {
                continue;    //for (i = 1; i <= rt1; i++)
            }
  
        }
        catch (CSerialException& pEx)
        {
            recv_len = 0;
            iRtn = EDC_ERROR;
            logsprintf("HighWayEDCComm():CSerialException %s ..", pEx.what() );
        }
        catch (exception& e)
        {
            recv_len = 0;
            iRtn = EDC_ERROR;
            logsprintf("HighWayEDCComm():exception %s ..", e.what() );
        }
        catch (...)
        {
            recv_len = 0;
            iRtn = EDC_ERROR;
            logsprintf("HighWayEDCComm():catch() Error ...");
        }
    }  // end of for

    if ( (i > rt1) || recv_len ==0 || iEndChar==0 )
    {
        *rPtrLen = recv_len;
        iRtn = EDC_ERROR; // EDCTIMEOUT;
        logsprintf("HighWayEDCComm():Receive Data TimeOut Or catch() Error -(結束接收) ... " );
    }

    //ERR_END:
        
    //logsprintf("HighWayEDCComm() -> result=%s", iRtn==SUCCEED ? "SUCCEED": "FAILURE");
  
    return(iRtn);
    
}
//
//if (HighWayEDCComm(send_data, 1, recv_data, &i) == EDCTIMEOUT)



//EDC_CommR: Read Data from EDC device
//return: Read Bytes 
//pWDongleData: STX(1)+ DATA[]+ ETX(1)+ LRC(2) || NAK
//注意當收到 NAK, 
//1. 通訊電文內容長度有錯.
//2. 電文內容 LRC 有錯.
//3. Commnd Code 有錯.

int __fastcall cLsEDC::EDC_CommR(int iTranType, LPSTR pWDongleData, int* piWDongleDataLength, int ComTimeOut)
{
    string tmp;
    int AvailableBytes = 0;
    int ReadBytes = 0;
    UINT iucharenqdateLen;

    //CHAR uRcv[MAX_PACK_DATA_SIZE + 1], utmpcharenqdate[MAX_PACK_DATA_SIZE + 1];
    CHAR* uRcv, * utmpcharenqdate;
    uRcv = (char*)malloc(MAX_PACK_DATA_SIZE + 1);
    utmpcharenqdate = (char*)malloc(MAX_PACK_DATA_SIZE + 1);
    //free(uRcv);
    //free(utmpcharenqdate);

    memset(uRcv, 0, sizeof(uRcv));
    memset(utmpcharenqdate, 0, sizeof(utmpcharenqdate));
    memset(pWDongleData, 0, sizeof(pWDongleData));

    *piWDongleDataLength = 0;
    iucharenqdateLen = 0;

    int itimeoutcnt, iETXOK, icnt;
    icnt = iETXOK = 0;

    if (ComTimeOut == 0)
        ComTimeOut = sEdcSet.TscComReqTimeOut;                  //TscComReqTimeOut: 每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms

    itimeoutcnt = ComTimeOut / sEdcSet.TscComReceiveInterval;   //TscComReceiveInterval: 每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2)) 之偵測 Interval default:3ms

    if (itimeoutcnt < 10)
        itimeoutcnt = 10;


    Debuglogsprintf("EDC_CommR(): Start Read Data Set ComTimeOut=%d , sEdcSet.TscComReceiveInterval=%d, and itimeoutcnt=%d",
        ComTimeOut, sEdcSet.TscComReceiveInterval, itimeoutcnt);
    //DebugLogMsg(tmp);

    while (1)  // read data unteil ETX
    {
        if (ComConnected == true)
        {
            Sleep(sEdcSet.TscComReceiveInterval);
            icnt++;
            AvailableBytes = SCOMx.BytesWaiting();
            if (AvailableBytes > 0)
            {
                ReadBytes = SCOMx.Read(utmpcharenqdate, MAX_PACK_DATA_SIZE);  // AvailableBytes);   //注意AvailableBytes,會漏接
                if (ReadBytes > 0)
                {
                    memcpy(&uRcv[iucharenqdateLen], utmpcharenqdate, ReadBytes);
                    //ucharenqdate=ucharenqdate(ReadBytes);
                    iucharenqdateLen += ReadBytes;

                    if (uRcv[0] == NAK)
                    {
                        //iStatus = NAK;
                        logsprintf("EDC_CommR():<-(接收NAK)(%s)", logStr2Hex(uRcv, iucharenqdateLen).c_str());
                        iETXOK = 1;
                    }
                    else if (uRcv[iucharenqdateLen - 3] == ETX)
                    {
                        //STX(1)+ DATA(226)+ ETX(1)+ LRC(2)
                        iETXOK = 1;
                    }


                }//if(ReadBytes>0)

            }//if(AvailableBytes>0)
         // else
         //   {
         //    break;
         //   }

            if (icnt > itimeoutcnt || iETXOK || iucharenqdateLen > MAX_PACK_DATA_SIZE || ReadBytes > 0)    //2017/06/16 Lu Update Add ==> ReadBytes > 0
                break;

        }//if( Connected==true)
        else
        {
            icnt = 9999;
            iucharenqdateLen = 0;
            logsprintf("EDC_CommR(): ComConnected=False,, Com Port Error..... ");
            break;
        }
    } // end of while

    memcpy(pWDongleData, uRcv, iucharenqdateLen);
    *piWDongleDataLength = iucharenqdateLen;

    if (icnt > itimeoutcnt)
    {
        logsprintf("EDC_CommR():End Read Data(%s)(%d), Read Data TimeOut=%d...",
            logStr2Hex(pWDongleData, iucharenqdateLen).c_str(), iucharenqdateLen, ComTimeOut);
        //DebugLogMsg(tmp);
        //RtnLogMsg(tmp);
    }
    else
    {
        if (iucharenqdateLen > 0)
        {
            Debuglogsprintf("EDC_CommR():End Read Data(%s)(%d), iETXOK=%d", logStr2Hex(pWDongleData, iucharenqdateLen).c_str(), iucharenqdateLen, iETXOK);
            //DebugLogMsg(tmp);
        }
    }

    free(uRcv);  
    free(utmpcharenqdate);   

    return(iucharenqdateLen);

}


void _fastcall cLsEDC::SetHostResponseCode(int iRtn)
{
    string s;
    s = _IntToHex(iRtn).c_str();
    memcpy(ucHostResponseCode, s.c_str(), s.length());
    //ucHostResponseCode[ s.length() ] = 0;

}


/***************************************************************************************************************************************************/


int __fastcall cLsEDC::EdcApiCall(int TranType, int TMode, byte* lpbInData, byte* lpbOutData, int iAmt, byte* Cmd)
{
    int iRtn = REQUEST_NOT_OK;
    string s;
    s = string((LPSTR)lpbInData);

    logsprintf("★★   Start EdcApiCall: EDCTranType=%d , InputData(%s). ★★", TranType, s.c_str());

    EDCApiInit(TranType, iAmt, s);

    iRtn=InitComPort();
    if (iRtn != SUCCEED)
    {
        SetHostResponseCode(iRtn);
        return (iRtn);
    }

   //if( TranType>699 && TranType<799 )    //EDC ComPor IO
    switch (TranType)
    {
        
    //case 700:  // FETC 查詢
    //case 706:  //異常交易補登 0x60/0x61/0x40/0x50   2010/10/13 Update
    //    iRtn = Fun_HighWayEDC_A080(TranType, AddValue);
    //    break;
    //case 701:  // FETC 加值
    //    iRtn = Fun_HighWayEDC_A140(TranType, AddValue);
    //    break;
    //case 702:  // FETC 加值取消
    //    iRtn = Fun_HighWayEDC_A120(TranType, AddValue);
    //    break;
    //case 703:  // "FE01 信用卡卡片查詢" , "FE00信用卡交易終止"   
    //    iRtn = Fun_HighWayEDC_FE01(TranType, TMode, iAmt, (LPSTR)lpbInData);
    //    break;
    case 704:  //信用卡交易 0010:信用卡, 0011:紅利 ,8010:信用卡, 2010:銀聯, 3010:SmartPay
        iRtn = Fun_HighWayEDC_0x32_0x40(TranType, TMode, iAmt, (LPSTR)lpbInData);
        break;
    case 705:  //信用卡退貨 0030, 0031
        iRtn = Fun_HighWayEDC_Cancel(TranType, TMode, iAmt, (LPSTR)lpbInData);
        break;
    case 707:   // "FE01, FE02 FE03, FE04  SmartPay卡片查詢"     ; FE04 Lusung Add 2018/06/01
        iRtn = Fun_HighWayEDC_FE02(TranType, TMode, iAmt, (LPSTR)lpbInData);
        break;
    case 708:  // FETC 存在
        iRtn = HighWayEDC_Exists(TranType, TMode, iAmt, (LPSTR)lpbInData);
        break;
    case 709:  // FETC 結帳
        iRtn = Fun_HighWayEDC_A090(TranType, TMode, iAmt, (LPSTR)lpbInData);
        break;
    case 710: //電簽啟動訊息
        iRtn = Fun_HighWayEDC_Sig(TranType, TMode, iAmt, (LPSTR)lpbInData);
        break;

    case 711: //電簽確認訊息
        iRtn = Fun_HighWayEDC_Confirm(TranType, TMode, iAmt, (LPSTR)lpbInData);
        break;
    default:
        iRtn = REQUEST_NOT_OK;
        SetHostResponseCode(iRtn);
    }

    CloseComPort();

    /**
    iRtn = CloseComPort();
    if (!iRtn)
    {
        SetHostResponseCode(iRtn);
        return (iRtn);
    }
    **/

    memcpy(lpbOutData, ucTscData, iTscDataLen);

    logsprintf("★★   END EdcApiCall: EDCTranType=%d ,Rtn(%d:x%04X).    ★★", EDCTranType, iRtn, iRtn);

    return (iRtn);

}

/***************************************************************************************************************************************************/

/***************************************************************************************************************************************************/

/***************************************************************************************************************************************************/

//信用卡退貨 : 0030, 0031, 8030
//銀聯卡交易 : 消費,退貨  2010,2030
//晶片金融卡 : 消費,退貨  3010,3030

int __fastcall cLsEDC::Fun_HighWayEDC_Cancel(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData)
{
    //HWEDC_DataFormat1 data_0x30; //收銀機起始訊息格式 HWEDC_DataFormat1 (長度 234, 無頭尾)
    char data_0x30[512]; //用 HWEDC_DataFormat1 結構變數塞資料太累,故還是用 sprintf 方式塞資料
    char send_data[512]; //data len = 234 + 4 = 238
    char recv_data[512]; //data len = 226 + 4 = 230
    //char LRC[3];
    HWEDC_Data_0x40 InputData_0x40, data_0x40; //刷卡機交易回應訊息格式 HWEDC_DataFormat3 (長度 226, 無頭尾)
    
    HWEDC_Data_0x40_spec1 data_0x40_special, InputData_0x40_special;  //刷卡機交易回應訊息格式之特別欄位格式六 (為取出餘額等資訊)
    HWEDC_Data_0x40_spec10  data_0x40_specialABC;
    HWEDC_Data_0x40_spec11  data_0x40_specialEDC;

    int i , iRtn , iEcrNo, iTimeOut;
    string  s, sTmpBankType; //, sTmpTenCode, sTmpEcrNo, sTmpSalesNo, sTmpInvoNo, sTmpTranNo, sTmpTranCode, sTmpTimeOut, sTmpBankType;

    //初始變數
    memset(data_0x30, '\0', sizeof(data_0x30));
    memset(send_data, '\0', sizeof(send_data));
    memset(recv_data, '\0', sizeof(recv_data));

    // 20160108  ADD %152 Byte
    // DongleApiInputData=>  DongleApiInputData.sprintf("%-6s%-8s%-8s%-6s%-10s%14s%06d%4s%04d",
    //                       sTenCode,sEcrNo,sTranNo,sSalesNo,sInvoNo, dttm, iAddValue, sTranCode, iTimeOut);
    s = string(lpInData);
    sTenCode = s.substr(1, 6);
    sEcrNo = s.substr(7, 8);
    sTranNo = s.substr(15, 8);
    sSalesNo = s.substr(23, 6);
    sInvoNo = s.substr(29, 10);
    //14
    //6
    sTranCode = s.substr(59, 4);    //(58,4);     2013/05/22
    sTimeOut = s.substr(63, 4);     //(62,4);
    sTmpBankType = s.substr(67, 4);

    string sChrInvoNo, s0X40Data;
    sChrInvoNo = s.substr(71, 10);     //(67,4)

    s0X40Data = s.substr(81, 226);  // 0x40 Data  226 Byte  (77,226)

    memcpy(&InputData_0x40, s0X40Data.c_str(), sizeof(InputData_0x40));
    memcpy(&InputData_0x40_special, &InputData_0x40.field_specil, sizeof(InputData_0x40_special));    // 信用卡
    memcpy(&data_0x40_specialABC, &InputData_0x40.field_specil, sizeof(InputData_0x40_special));    // 銀聯卡交易 : 消費,退貨   2010,2030
    memcpy(&data_0x40_specialEDC, &InputData_0x40.field_specil, sizeof(InputData_0x40_special));    // 晶片金融卡 : 消費,退貨  3010,3030

    iEcrNo = _StrToInt(sEcrNo);
    iTimeOut = _StrToInt(sTimeOut);
 
    string sInputTranCode;
    _Strprintf(sInputTranCode,"%4.4s", InputData_0x40.field_06);

   // switch (_Trim(sInputTranCode) )
   //// {
     //}


    if (_Trim(sInputTranCode) == "0011")       //信用卡退貨
        sTranCode = "0031";
    else if (_Trim(sInputTranCode) == "2010")  // 銀聯卡退貨
        sTranCode = "2030";
    else if (_Trim(sInputTranCode) == "3010")  // 晶片金融卡
        sTranCode = "3030";
    else if (_Trim(sInputTranCode) == "8010")  // 信用卡退貨新版
        sTranCode = "8030";
    else
        sTranCode = "8030";               //信用卡退貨  2018/02/01 確認一率改為 8030

   logsprintf("Fun_HighWayEDC_Cancel: 信用卡退貨L01  DongleApiInputData(%s), InputTranCode(%s), TranCode(%s)",
        s.substr(1, 80).c_str(), sInputTranCode.c_str(), sTranCode.c_str());
  
   logsprintf("Fun_HighWayEDC_Cancel: 信用卡退貨L02  InputData_0x40(%s), DataLen=%d, TranModeSw=%d",
        logStr2Hex((char*)&InputData_0x40, sizeof(InputData_0x40)), sizeof(InputData_0x40), TranModeSw);
    

    if (_Trim(sTranCode) == "2030")
    {
        logsprintf("Fun_HighWayEDC_Cancel: 銀聯卡退貨L03  金額(%d), 原交易序號(%8.8s), 原交易參考號(%12.12s), 原始卡號(%30.30s), 調閱編號(%6.6s), 銀聯卡交易序號(%6.6s), 交易授權碼(%6.6s).  ",
            iAddValue, InputData_0x40.field_04, data_0x40_specialABC.field_02, data_0x40_specialABC.field_01, data_0x40_specialABC.field_03, data_0x40_specialABC.field_13, data_0x40_specialABC.field_05);
    }
    else   if (_Trim(sTranCode) == "3030")
    {
        logsprintf("Fun_HighWayEDC_Cancel: 晶片金融卡退貨L03  金額(%d), 原交易序號(%8.8s), 原交易參考號(%12.12s), 原始卡號(%30.30s), 調閱編號(%6.6s).  ",
            iAddValue, InputData_0x40.field_04, data_0x40_specialEDC.field_02, data_0x40_specialEDC.field_01, data_0x40_specialEDC.field_03);
    }
    else
    {
        logsprintf("Fun_HighWayEDC_Cancel: 信用卡退貨L03  金額(%d), 原交易序號(%8.8s), 原交易參考號(%12.12s), 原始卡號(%30.30s), 調閱編號(%6.6s). ",
            iAddValue, InputData_0x40.field_04, InputData_0x40_special.field_02, InputData_0x40_special.field_01, InputData_0x40_special.field_03);
    }


   
    if (TranModeSw == 1)
    {
        iRtn = SUCCEED;
        SetHostResponseCode(iRtn);
        memset(data_0x30, '0', 234);
         iTscDataLen = 234;
        memcpy(ucTscData, data_0x30, iTscDataLen);
        logsprintf("Fun_HighWayEDC_Cancel:TranMode(%s),TscDataLen(%d) ", ucTscData, iTscDataLen);
        //RtnLogMsg(s);
        Sleep(2000);
        return(iRtn);
    }
 
    if (_Trim(sTranCode) == "2030")   // 銀聯卡退貨
    {
        sprintf_s(data_0x30,"%c%-6s%-8s%-8s%-6s%-4s%010d00  %-10s%08d%015d%03d%8.8s%12.12s%-30s%s%-2s%-6s%-8s%-8s%-6s%30.30s%4.4s%6.6s%30s",
            0x30,                               // 訊息代碼        固定為 CHAR(30)
            sTenCode.c_str(),                   // 店號            右靠左補零
            sEcrNo.c_str(),                     // 收銀機代號      右靠左補零
            sTranNo.c_str(),                    // 收銀機交易序號
            sSalesNo.c_str(),                   // 收銀員代碼      左靠右補空白
            sTranCode.c_str(),                  // 交易代碼        消費
            iAddValue,                          // 交易金額        整數 10 , 小數 2
            sInvoNo.c_str(),                         // 發票號碼
            0,                                  // 刷卡機端末代號  右靠左補零
            0,                                  // 刷卡機商店代號  右靠左補零
            4,                                  // 刷卡機 Timeout  秒
            InputData_0x40_special.field_03,     //        // 交易序號 InputData_0x40.field_04,, 調閱編號 InputData_0x40_special.field_03
            InputData_0x40_special.field_02,       // 交易參考號碼    (訂單編號)
            "",                                 // 卡片卡號        補空白
            " ",                                // 刷卡機狀態      補空白
            "10",                                 // 業務代號
            sTenCode.c_str(),                     // 店號
            sEcrNo.c_str(),                       // 機號
            sTranNo.c_str(),
            "",                                   // 保留欄位
            data_0x40_specialABC.field_01,      // 特別欄位  卡號
            data_0x40_specialABC.field_15,      // 特別欄位  銀聯卡交易日期
            data_0x40_specialABC.field_05,      // 特別欄位  交易授權碼,   data_0x40_specialABC.field_13, 銀聯卡交易序號
            ""
        );



    }
    else if (_Trim(sTranCode) == "3030")
    {
        sprintf_s(data_0x30,"%c%-6s%-8s%-8s%-6s%-4s%010d00  %-10s%08d%015d%03d%8.8s%12.12s%-30s%s%-2s%-6s%-8s%-8s%-6s%30.30s%12.12s%28s",
            0x30,                               // 訊息代碼        固定為 CHAR(30)
            sTenCode.c_str(),                   // 店號            右靠左補零
            sEcrNo.c_str(),                   // 收銀機代號      右靠左補零
            sTranNo.c_str(),                    // 收銀機交易序號
            sSalesNo.c_str(),                   // 收銀員代碼      左靠右補空白
            sTranCode.c_str(),                  // 交易代碼        消費
            iAddValue,                          // 交易金額        整數 10 , 小數 2
            sInvoNo.c_str(),                         // 發票號碼
            0,                                  // 刷卡機端末代號  右靠左補零
            0,                                  // 刷卡機商店代號  右靠左補零
            4,                                  // 刷卡機 Timeout  秒
            InputData_0x40_special.field_03,     //        // 交易序號 InputData_0x40.field_04,, 調閱編號 InputData_0x40_special.field_03
            InputData_0x40_special.field_02,       // 交易參考號碼    (訂單編號)
            "",                                 // 卡片卡號        補空白
            " ",                                // 刷卡機狀態      補空白
            "10",                                 // 業務代號
            sTenCode.c_str(),                     // 店號
            sEcrNo.c_str(),                       // 機號
            sTranNo.c_str(),
            "",                                   // 保留欄位
            InputData_0x40_special.field_01,      // 特別欄位  卡號
            InputData_0x40_special.field_02,      // 特別欄位  原交易參考號
            ""
        );


    }
    else      //信用卡退貨
    {
        sprintf_s(data_0x30,"%c%-6s%-8s%-8s%-6s%-4s%010d00  %-10s%08d%015d%03d%8.8s%12.12s%-30s%s%-2s%-6s%-8s%-8s%6s%30.30s%40s",
            0x30,                               // 訊息代碼        固定為 CHAR(30)
            sTenCode.c_str(),                   // 店號            右靠左補零
            sEcrNo.c_str(),                   // 收銀機代號      右靠左補零
            sTranNo.c_str(),                    // 收銀機交易序號
            sSalesNo.c_str(),                   // 收銀員代碼      左靠右補空白
            sTranCode.c_str(),                  // 交易代碼        消費
            iAddValue,                          // 交易金額        整數 10 , 小數 2
            sInvoNo.c_str(),                         // 發票號碼
            0,                                  // 刷卡機端末代號  右靠左補零
            0,                                  // 刷卡機商店代號  右靠左補零
            4,                                  // 刷卡機 Timeout  秒
            InputData_0x40_special.field_03,     //        // 交易序號 InputData_0x40.field_04,, 調閱編號 InputData_0x40_special.field_03
            InputData_0x40_special.field_02,       // 交易參考號碼    (訂單編號)
            "",                                 // 卡片卡號        補空白
            " ",                                // 刷卡機狀態      補空白
            "10",                                 // 業務代號
            sTenCode.c_str(),                     // 店號6
            sEcrNo.c_str(),                       // 機號8
            sTranNo.c_str(),                      //序號8
            sTmpBankType.c_str(),                    //銀行代碼4白右靠  +補2個空 
            InputData_0x40_special.field_01,         // 特別欄位
            ""                                       // 40spec
        );

    }

    logsprintf("Fun_HighWayEDC_Cancel: data_0x30(%234.234s), DataLen=234", data_0x30);
    //送出命令  
    //sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3+1
    //sEdcSet.TscComReqTimeOut;   每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms
    iRtn = HighWayEDCComm(data_0x30, 234, recv_data, &i, sEdcSet.TscComReqTimeOut);
    SetHostResponseCode(iRtn);

    if (iRtn == SUCCEED)
    {
        //memcpy(&data_0x40, recv_data, i); //234 電子發票載具   2018/01/22   Lu update
        //iTscDataLen = sizeof(data_0x40);
        //memcpy(ucTscData, &data_0x40, iTscDataLen);
        memcpy(send_data, recv_data, i);
        logsprintf("Fun_HighWayEDC_Cancel:Rcv(%s), DataLen=%d", logStr2Hex(ucTscData, iTscDataLen).c_str(), iTscDataLen);
    }
    else
    {
        logsprintf("HighWayEDC_Send_Ex:信用卡退貨 -> (通訊第一階段失敗0x30) ResponseCode=%s, rtn=%d", ucHostResponseCode, iRtn);
        return(iRtn);
    }

    memset(recv_data, '\0', sizeof(recv_data));
    int iX40dataLen = 226;
    if (_Trim(sTranCode) == "8030")
        iX40dataLen = 400;

    //sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3+1
    //sEdcSet.TscComReqTimeOut;   每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms
    iRtn = HighWayEDCComm(send_data, iX40dataLen, recv_data,  &i, sEdcSet.TscComReqTimeOut);    //21??
    SetHostResponseCode(iRtn);

    if (iRtn == SUCCEED)
    {
        memcpy(&data_0x40, recv_data, 400);    // 電子發票載具   2018/01/22   Lu update
        memcpy(&data_0x40_special, &data_0x40.field_specil, sizeof(data_0x40_special));
        iTscDataLen = sizeof(data_0x40);
        memcpy(ucTscData, &data_0x40, iTscDataLen);
        logsprintf("Fun_HighWayEDC_Cancel:Rcv(%s), DataLen=%d", logStr2Hex(ucTscData, iTscDataLen), iTscDataLen);
    }
    else
    {
        logsprintf("HighWayEDC_Send_Ex:信用卡退貨 -> (通訊第一階段失敗0x30) ResponseCode=%s, rtn=%d", ucHostResponseCode, iRtn);
        return(iRtn);
    }


    logsprintf("Fun_HighWayEDC_Cancel: EDC 退貨完成 ucTscData_0x40(%s), DataLen=%d",  logStr2Hex(ucTscData, iTscDataLen), iTscDataLen);

    logsprintf("Fun_HighWayEDC_Cancel:信用卡退貨 回應代碼(%4.4s) 卡號(%30.30s)- 原始交易金額(%12.12s),實際交易金額(%12.12s),本次折抵點數(%9.9s),本次折抵金額(%12.12s),剩餘點數(%9.9s), 電子發票載具(%50.50s) ->>信用卡退貨交易成功.",
        data_0x40.field_07, data_0x40_special.field_01, data_0x40_special.field_06, data_0x40_special.field_07, data_0x40_special.field_08, data_0x40_special.field_09, data_0x40_special.field_10, data_0x40.field_speci2);
    RtnLogMsg(s);

    return(iRtn);



   /****

    //strncpy(LRC, LRC_VALUE(data_0x30).c_str(), 2);
    //LRC[2] = 0;

    //打包 send_data 內容
    //sprintf(send_data, "%c%-234s%c%-2s", STX, data_0x30, ETX, LRC);
    //send_data[238] = 0;

    

    //送出命令 & 做通訊第一階段判斷
    if (HighWayEDC_Send_Ex(send_data, 238, iTimeOut) != SUCCEED)
    {
        //錯誤處理
        RtnLogMsg("HighWayEDC_Send_Ex:信用卡退貨 -> (通訊第一階段失敗0x30)");

        s = "2011";  // 2011 通訊失敗
        memcpy(ucHostResponseCode, s.c_str(), 4);
        ucHostResponseCode[4] = 0;
        //RelClrShowPMSG(); //2005/07/05
        //show_down_scr2(" "," "," ");
        //show_down_scr2("請按[清除]鍵"," ","e通卡 加值機讀卡失敗");
        //err_code=90; crs_buzzer_on();
        return(2);    //"讀寫器錯誤!!"
    }
    memcpy(recv_data, send_data, 234);

    //memset(send_data, '\0', sizeof(send_data));
    //memset(recv_data, '\0', sizeof(recv_data));

    int iX40dataLen = 226;
    if (Trim(sTmpTranCode) == "8030")
        iX40dataLen = 400;
    if (Fun_HighWayEDC_0x40(recv_data, iX40dataLen, 21) != SUCCEED)
    {
        //錯誤處理
        RtnLogMsg("Fun_HighWayEDC_0x40:EDC退貨 -> (通訊失敗0x40)");

        s = "2011";  // 2011 通訊失敗
        memcpy(ucHostResponseCode, s.c_str(), 4);
        ucHostResponseCode[4] = 0;
        return(2);    //"讀寫器錯誤!!"
    }


    //取出無頭尾的 data_0x40
    //recv_data[234]=0;
    memcpy(&data_0x40, recv_data, 400);    // 電子發票載具   2018/01/22   Lu update
    memcpy(&data_0x40_special, &data_0x40.field_specil, sizeof(data_0x40_special));


    if (data_0x40.field_01[0] != 0x40)
    {
        //錯誤處理
        RtnLogMsg("Fun_HighWayEDC_0x40:EDC退貨 -> (通訊失敗.. 回覆非0x40電文.)");
        s = "2011";  // 2011 通訊失敗
        memcpy(ucHostResponseCode, s.c_str(), 4);
        ucHostResponseCode[4] = 0;
        //iTscDataLen=0;
        //memset(ucTscData, 0, sizeof(ucTscData) );
        return(2);    //"讀寫器錯誤!!"

    }

   
    iTscDataLen = 400;   //226 電子發票載具   2018/01/22   Lu update
    memcpy(ucTscData, &data_0x40, iTscDataLen);

    s.sprintf("Fun_HighWayEDC_Cancel: EDC 退貨完成 ucTscData_0x40(%s), DataLen=%d",
        RtnLogMs_Str2Hex(ucTscData, iTscDataLen), iTscDataLen);
    //DebugLogMsg(s);
    RtnLogMsg(s);

       s.sprintf("Fun_HighWayEDC_Cancel:信用卡退貨 回應代碼(%4.4s) 卡號(%30.30s)- 原始交易金額(%12.12s),實際交易金額(%12.12s),本次折抵點數(%9.9s),本次折抵金額(%12.12s),剩餘點數(%9.9s), 電子發票載具(%50.50s) ->>信用卡退貨交易成功.",
        data_0x40.field_07, data_0x40_special.field_01, data_0x40_special.field_06, data_0x40_special.field_07, data_0x40_special.field_08, data_0x40_special.field_09, data_0x40_special.field_10, data_0x40.field_speci2);
    RtnLogMsg(s);


    return(0);    // SUCCEED
    *******/
}

/***************************************************************************************************************************************************/
// iTranType :704:  信用卡交易
//信用卡交易 0010:信用卡, 0011:信用卡紅利 ,8010:信用卡, 2010:銀聯, 3010:SmartPay
int __fastcall cLsEDC::Fun_HighWayEDC_0x32_0x40(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData)
{
    char data_0x32[512]; //用 HWEDC_DataFormat1 結構變數塞資料太累,故還是用 sprintf 方式塞資料
    char send_data[512]; //data len = 234 + 4 = 238
    char recv_data[512]; //data len = 226 + 4 = 230
   
    HWEDC_Data_0x40 data_0x40; //刷卡機交易回應訊息格式  總長度 226 + 50 + 124 = 400 
    HWEDC_Data_0x40_spec1 data_0x40_special; //刷卡機交易回應訊息格式之特別欄位格式六 (為取出餘額等資訊)
    int i, iEcrNo, iTimeOut, iRtn;
    string  s;


    //初始變數
    s = string(lpInData);
    memset(data_0x32, '\0', sizeof(data_0x32));
    memset(send_data, '\0', sizeof(send_data));
    memset(recv_data, '\0', sizeof(recv_data));
    memset(&data_0x40, ' ', sizeof(data_0x40));    // 電子發票載具   2018/01/22   Lu update
    sCard_no = "";
    
    // 20160108  ADD %152 Byte
    // DongleApiInputData=>  DongleApiInputData.sprintf("%-6s%-8s%-8s%-6s%-10s%14s%06d%4s%04d",
    //                       sTenCode,sEcrNo,sTranNo,sSalesNo,sInvoNo, dttm, iAddValue, sTranCode, iTimeOut);
    
    //傳給InLine資料 = 009939000000031210009699999 XY9990010220180612061913000077801000300012123456789012345678901234567890     2018/05/31  100 Byte
    sTenCode = s.substr(0, 6);
    sEcrNo = s.substr(6, 8);
    sTranNo = s.substr(14, 8);
    sSalesNo = s.substr(22, 6);
    sInvoNo = s.substr(28, 10);
    //38,14
    //52,6         
    sTranCode = s.substr(58, 4);    //(58,4);     2013/05/22
    sTimeOut = s.substr(62, 4);     //(62,4);
    sBandCode = _Trim(s.substr(66, 4));       //(66,4);     2017/07/31
    sCard_no = _Trim(s.substr(70, 30));       //2018/05/31

    iEcrNo = _StrToInt(sEcrNo);
    //iTimeOut = _StrToInt(sTimeOut);
  
    iTimeOut = iTscX40TimeOut/1000;          // default 35000 Sec  Ref INI  刷卡機TimeOut 秒為單位
    

    logsprintf("★ Fun_HighWayEDC_0x32_0x40: %s DongleApiInputData=%s, AddValue=%d, sTranCode=%s. ★", iTranType == 706 ? "異常交易補登" : "信用卡交易", s.c_str(), iAddValue, sTranCode.c_str());
   // RtnLogMsg(s);

    if (TranModeSw == 1)
    {
        iRtn = SUCCEED;
        SetHostResponseCode(iRtn);
        memset(&data_0x32, '0', 234);
        iTscDataLen = 234;
        memcpy(ucTscData, &data_0x32, iTscDataLen);
        logsprintf("Fun_HighWayEDC_0x32_0x40:TranMode(%s),TscDataLen(%d) ", ucTscData, iTscDataLen);
        Sleep(2000);
        return(iRtn);
    }


    //設定 data_0x32 內容 (用 HWEDC_DataFormat1 結構變數塞資料太累,故還是用 sprintf 方式塞資料)
    sprintf_s(data_0x32, "%c%-6s%-8s%-8s%-6s%-4s%010ld00  %-10s%08d%015d%03d%8s%-12s%-30s%s%-2s%-6s%-6s%02d%-8s%-2s%-4.4s%-9.9s%-61s",
        0x32,                             // 訊息代碼        固定為 CHAR(32)
        sTenCode.c_str(),                   // 店號            右靠左補零
        sEcrNo.c_str(),                     // 收銀機代號      右靠左補零
        sTranNo.c_str(),                    // 收銀機交易序號
        sSalesNo.c_str(),                   // 收銀員代碼      左靠右補空白
        sTranCode.c_str(),                  // 交易代碼
        iAddValue,                          // 交易金額        整數 10 , 小數 2
        sInvoNo.c_str(),                    // 發票號碼
        0,                                  // 刷卡機端末代號  右靠左補零
        0,                                  // 刷卡機商店代號  右靠左補零
        iTimeOut,                           // 刷卡機 Timeout  秒
        " ",                                  // 交易序號
        "",                                 // 交易參考號碼    (訂單編號)
        "",                                 // 卡片卡號        補空白
        " ",                                // 刷卡機狀態      補空白
        "10",                                 // 業務代號
        sTenCode.c_str(),                     // 店號
        "000000",                             //
        iEcrNo,                               // 機號
        sTranNo.c_str(),                   //收銀機交易序號
        "",                                 // 業務需求欄位
        sBandCode.c_str(),                  // 銀行代碼
        sCard_no.c_str(),                   // 特別欄位1    2018/06/01
        ""                                  // 特別欄位2
    );
    
    iTimeOut = iTscX40TimeOut + 5000;  //// default 35000 Sec  ==> 40000
    
    logsprintf("Fun_HighWayEDC_0x32_0x40:_0x32(%234.234s), DataLen=234 , TimeOut=%d", data_0x32, iTimeOut);
   
    //送出命令  
    //sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3+1
    //sEdcSet.TscComReqTimeOut;   每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms
    iRtn = HighWayEDCComm(data_0x32, 234, recv_data, &i, iTimeOut);
    SetHostResponseCode(iRtn);

    if (iRtn == SUCCEED)
    {
        memcpy(&data_0x40, recv_data, i); //234 電子發票載具   2018/01/22   Lu update
        iTscDataLen = sizeof(data_0x40);
        memcpy(ucTscData, &data_0x40, iTscDataLen);
        memcpy(&data_0x40_special, &data_0x40.field_specil, sizeof(data_0x40_special));
               
        logsprintf("Fun_HighWayEDC_0x32_0x40:ucTscData_0x40(%s), DataLen=%d", logStr2Hex(ucTscData, iTscDataLen).c_str(), iTscDataLen);
 
    }
    else
    {
        logsprintf("Fun_HighWayEDC_0x32_0x40:信用卡交易第二階段通訊->失敗0 ResponseCode=%s, rtn=%d ,X%04X", ucHostResponseCode, iRtn, iRtn);
        return(iRtn);     
    }

 
   // memcpy(&data_0x40, recv_data, 400); //234 電子發票載具   2018/01/22   Lu update
   // memcpy(&data_0x40_special, &data_0x40.field_specil, sizeof(data_0x40_special));

    //取出 data_0x40 的特別欄位
    //strncpy(data_0x40_special,data_0x40.field_specil,152);
    //memcpy(&data_0x40_special, &data_0x40.field_specil, 152);

    if (data_0x40.field_01[0] != 0x40)
    {
        iRtn = DATA_ERROR;
        SetHostResponseCode(iRtn);
        //錯誤處理
        RtnLogMsg("Fun_HighWayEDC_Trans:信用卡交易 -> (通訊失敗.. 回覆非0x40電文.)");
        return(iRtn);     

    }

    //ucHostResponseCode
    iTscDataLen = 400;  //234 電子發票載具   2018/01/22   Lu update
    memcpy(ucTscData, &data_0x40, iTscDataLen);
    logsprintf("Fun_HighWayEDC_0x32_0x40: 信用卡交易 ucTscData_0x40(%s), DataLen=%d",  logStr2Hex(ucTscData, iTscDataLen).c_str(), iTscDataLen);
  
    //特別格式第六種的欄位四
    //int icard_type=atoi(data_0x40_special.field_04); //卡片類別="1"->銀行卡(信用卡加值)  卡片類別="2"->遠通卡(現金加值)

    logsprintf("Fun_HighWayEDC_0x32_0x40: 信用卡交易 回應代碼(%4.4s) 卡號(%30.30s)- 原始交易金額(%12.12s),實際交易金額(%12.12s),本次折抵點數(%9.9s),本次折抵金額(%12.12s),剩餘點數(%9.9s),簽名(%1.1s) ->信用卡通訊完成.",
        data_0x40.field_07, data_0x40_special.field_01, data_0x40_special.field_06, data_0x40_special.field_07, data_0x40_special.field_08, data_0x40_special.field_09,
        data_0x40_special.field_10, data_0x40_special.field_12);
    //RtnLogMsg(s);

    logsprintf("Fun_HighWayEDC_0x32_0x40: 信用卡交易  金額(%d), 交易序號(%8.8s), 交易參考號(%12.12s), 卡號(%30.30s), 調閱編號(%6.6s), 電子發票載具(%50.50s). ",
        iAddValue, data_0x40.field_04, data_0x40_special.field_02, data_0x40_special.field_01, data_0x40_special.field_03, data_0x40.field_speci2);
  
    //RtnLogMsg(s);

    return(iRtn);    // SUCCEED

}



/***************************************************************************************************************************************************/


/***************************************************************************************************************************************************/

//SmartPay查詢  0x30 <-> 0x31   // "FE01, FE02 FE03, FE04  SmartPay卡片查詢"     ; FE04 Lusung Add 2018/06/01
int __fastcall cLsEDC::Fun_HighWayEDC_FE02(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData)
{
    
    char data_0x30[512]; //用 HWEDC_DataFormat1 結構變數塞資料太累,故還是用 sprintf 方式塞資料
    char send_data[512]; //data len = 234 + 4 = 238
    char recv_data[512]; //data len = 226 + 4 = 230
    char LRC[3];
    HWEDC_Data_0x31 data_0x31;
    int i , iRtn, iEcrNo;

    string  s; // , sTmpTenCode, sTmpEcrNo, sTmpSalesNo, sTmpInvoNo, sTmpTranNo, sTmpTranCode, sTmpTimeOut;

    //初始變數
    s = string(lpInData);
    memset(data_0x30, '\0', sizeof(data_0x30));
    memset(send_data, '\0', sizeof(send_data));
    memset(recv_data, '\0', sizeof(recv_data));
    memset(&data_0x31, '\0', sizeof(data_0x31));


    sTenCode = s.substr(0, 6);
    sEcrNo = s.substr(6, 8);
    sTranNo = s.substr(14, 8);
    sSalesNo = s.substr(22, 6);
    sInvoNo = s.substr(28, 10);
                      //38, 14
                      //52, 6
    sTranCode = s.substr(58, 4);
    sTimeOut = s.substr(62, 4);    //(62,4);
        
    iEcrNo = _StrToInt(sEcrNo);
       
    if (sTranCode == "FE04")
    {
        logsprintf("★ Fun_HighWayEDC_FE02: %s DongleApiInputData=%s, AddValue=%d, sTranCode=%s. ★", "信用卡卡片查詢", s.c_str(), iAddValue, sTranCode.c_str());
    }
    else
    {

        logsprintf("★ Fun_HighWayEDC_FE02: %s DongleApiInputData=%s, AddValue=%d, sTranCode=%s. ★",
            sTranCode == "FE01" ? "信用卡卡片查詢" : (sTranCode == "FE02" ? "SmartPay查詢" : "電子發票載具查詢"),
            s.c_str(), iAddValue, sTranCode.c_str());
    }
    //RtnLogMsg(s);

    if (TranModeSw == 1)
    {
        iRtn = SUCCEED;
        SetHostResponseCode(iRtn);
        memset(&data_0x31, '0', 234);
        iTscDataLen = 234;
        memcpy(ucTscData, &data_0x31, iTscDataLen);
        logsprintf("Fun_HighWayEDC_FE02:TranMode(%s),TscDataLen(%d) ", ucTscData, iTscDataLen);
        Sleep(1000);
        return(iRtn);
    }


    //設定 data_0x30 內容 (用 HWEDC_DataFormat1 結構變數塞資料太累,故還是用 sprintf 方式塞資料)
    sprintf_s(data_0x30,"%c%-6s%-8s%-8s%-6s%-4s%010ld00  %-10s%08ld%015ld%03ld%08ld%-12s%-30s%s%-2s%-6s%-6s%02d%-8s%-6s%-70s",
        0x30,                               // 訊息代碼        固定為 CHAR(30)
        sTenCode.c_str(),                   // 店號            右靠左補零
        sEcrNo.c_str(),                     // 收銀機代號      右靠左補零
        sTranNo.c_str(),                    // 收銀機交易序號
        sSalesNo.c_str(),                    // 收銀員代碼      左靠右補空白
        sTranCode.c_str(),                   // FE01, Fe02, FE03 交易代碼
        iAddValue,                          // 交易金額        整數 10 , 小數 2
        sInvoNo.c_str(),                    // 發票號碼
        0,                                  // 刷卡機端末代號  右靠左補零
        0,                                  // 刷卡機商店代號  右靠左補零
        3,                                  // 刷卡機 Timeout  秒
        0,                                  // 交易序號
        "",                                 // 交易參考號碼    (訂單編號)
        "",                                 // 卡片卡號        補空白
        " ",                                // 刷卡機狀態      補空白
        "10",                                 // 業務代號
        sTenCode.c_str(),                     // 店號
        "000000",                             //
        iEcrNo,                               // 機號
        sTranNo.c_str(),                      //收銀機交易序號
        "000000",                                 // 業務需求欄位
        ""                                  // 特別欄位
    );


    //送出命令  
    //sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3+1
    //sEdcSet.TscComReqTimeOut;   每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms
    iRtn = HighWayEDCComm(data_0x30, 234, recv_data, &i, sEdcSet.TsccEnqIntervalSec);
    SetHostResponseCode(iRtn);
    if (iRtn == SUCCEED)
    {
        memcpy(&data_0x31, recv_data, i);
        iTscDataLen = sizeof(data_0x31);
        memcpy(ucTscData, &data_0x31, iTscDataLen);
        logsprintf("Fun_HighWayEDC_FE02:ucTscData_0x31(%s), DataLen=%d", logStr2Hex(ucTscData, iTscDataLen).c_str(), iTscDataLen);
               
        if (sTranCode == "FE01" || sTranCode == "FE02")
        {
            logsprintf("Fun_HighWayEDC_FE02:SmartPay查詢 卡號(%30.30s), 卡片類別(%c), field_specil(%70.70s) ->查詢成功.",
                data_0x31.field_14, data_0x31.field_specil[5], data_0x31.field_specil);
        }
        else  if (sTranCode == "FE03")
        {
            logsprintf("電子發票載具查詢 FE03: 卡號(%30.30s), 載具編號(%50.50s), 業務需求欄位(%30.30s), field_specil(%70.70s) ->查詢成功.",
                data_0x31.field_14, &data_0x31.field_specil[5], data_0x31.field_16, data_0x31.field_specil);
        }
        else    //FE04 Lusung Add 2018/06/01
        {
            logsprintf("電子發票載具查詢 FE04: 卡號(%30.30s), 載具編號(%8.8s), 業務需求欄位(%30.30s), field_specil(%70.70s) ->查詢成功.",
                data_0x31.field_14, &data_0x31.field_specil[7], data_0x31.field_16,  data_0x31.field_specil);
           // field_16[30];      // 業務需求欄位
        }
         
    }
    else
    {
        logsprintf("Fun_HighWayEDC_FE02:SmartPay查詢 -> (通訊第一階段失敗0x30) ResponseCode=%s, rtn=%d", ucHostResponseCode, iRtn);
    }
    
    return(iRtn);    // SUCCEED
}

/***************************************************************************************************************************************************/


/***************************************************************************************************************************************************/
//FETC is Exists 
//Check EDC connect status 
//return: SUCCEED

int  __fastcall cLsEDC::HighWayEDC_Exists(int iTranType,  int iTranModeSw, int iAddValue, LPSTR lpInData)
{
    string s;
    char send_data[239];  
    char recv_data[231];  
    //HWEDC_Data_0x40 data_0x40; //刷卡機交易回應訊息格式 HWEDC_DataFormat3 (長度 226, 無頭尾)
    //HWEDC_Data_0x40_spec6 data_0x40_special; //刷卡機交易回應訊息格式之特別欄位格式六 (為取出餘額等資訊)
    int i = 0;
    int iRtn = 0;

    //初始變數
    s = string(lpInData);
    //memset(data_0x30, '\0', sizeof(data_0x30));
    memset(send_data, '\0', sizeof(send_data));
    memset(recv_data, '\0', sizeof(recv_data));
    //memset(&data_0x40, ' ', sizeof(data_0x40));    // 電子發票載具   2018/01/22   Lu update

    // DongleApiInputData=>  DongleApiInputData.sprintf("%-6s%-8s%-8s%-6s%-10s%14s%010ld00",
    //                       sTenCode,sEcrNo,sTranNo,sSalesNo,sInvoNo, dttm, );

    sTenCode = s.substr(0, 6);
    sEcrNo = s.substr(6, 8);
    sTranNo = s.substr(14, 8);
    sSalesNo = s.substr(22, 6);
    sInvoNo = s.substr(28, 10);;
  
    logsprintf("HighWayEDC_Exists:EDC 驗證 iTranType=%d, TranModeSw=%d, DongleApiInputData=%s, iAddValue=%d", iTranType, iTranModeSw, s.c_str(), iAddValue);
    
    if (TranModeSw == 1)
    {
        iRtn = SUCCEED;
        s = _IntToHex(iRtn).c_str();
        iTscDataLen = s.length();
        SetHostResponseCode(iRtn);
        memset(ucTscData, '0', iTscDataLen);
        logsprintf("HighWayEDC_Exists:EDC 驗證 TranMode(%s),TscDataLen(%d) ", ucTscData, iTscDataLen);
        return(SUCCEED);
    }

    //sprintf_s(send_data, "%c", 0x60);
    send_data[0] = 0x60;
    send_data[1] = 0;
    
    //s = logStr2Hex(send_data, 1);
    logsprintf("HighWayEDC_Exists:EDC 驗證 send_data(%s), DataLen=%d", logStr2Hex(send_data, 1).c_str(), 1);
    
    //送出命令 & 做通訊第一階段判斷
    //sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3+1
    //sEdcSet.TscComReqTimeOut;   每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms
    iRtn = HighWayEDCComm(send_data, 1, recv_data, &i, sEdcSet.TsccEnqIntervalSec);
    s = _IntToHex(iRtn).c_str();
    iTscDataLen = s.length();
    SetHostResponseCode(iRtn);
    
    if (iRtn == EDCTIMEOUT )
       RtnLogMsg("HighWayEDC_Exists:EDC 驗證 -> (通訊失敗), e通卡機不存在!!");
    else
       logsprintf("HighWayEDC_Exists:EDC 驗證 SUCCEED");
 

    return(iRtn);    // SUCCEED

}


/**************************************************************************************************************************************************/


//FETC 結帳
int  __fastcall cLsEDC::Fun_HighWayEDC_A090(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData) //結帳
{
    char data_0x30[235]; //用 HWEDC_DataFormat1 結構變數塞資料太累,故還是用 sprintf 方式塞資料
    char send_data[239]; //data len = 234 + 4 = 238
    char recv_data[231]; //data len = 226 + 4 = 230
    char LRC[3];
    HWEDC_Data_0x40 data_0x40; //刷卡機交易回應訊息格式 HWEDC_DataFormat3 (長度 226, 無頭尾)
    //HWEDC_Data_0x40_spec6 data_0x40_special; //刷卡機交易回應訊息格式之特別欄位格式六 (為取出餘額等資訊)
    int iRtn=0;
    int i = 0;
    string s;
   

    //初始變數
    memset(data_0x30, '\0', sizeof(data_0x30));
    memset(send_data, '\0', sizeof(send_data));
    memset(recv_data, '\0', sizeof(recv_data));
    memset(&data_0x40, ' ', sizeof(data_0x40));    // 電子發票載具   2018/01/22   Lu update
   

    // DongleApiInputData=>  DongleApiInputData.sprintf("%-6s%-8s%-8s%-6s%-10s%14s%010ld00",
    //                       sTenCode,sEcrNo,sTranNo,sSalesNo,sInvoNo, dttm, );
       

    sTenCode = "000000";
    sEcrNo = "00000000";
    sTranNo = "00000000";
    sSalesNo = "000000";
    sInvoNo = "0000000000";

    logsprintf("Fun_HighWayEDC_A090:EDC 結帳 ");
  
    if (TranModeSw == 1)
    {
      
        SetHostResponseCode(iRtn);
        memset(&data_0x40, '0', 226);
        iTscDataLen = 226;
        memcpy(ucTscData, &data_0x40, iTscDataLen);
        logsprintf("Fun_HighWayEDC_A090:TranMode(%s),TscDataLen(%d) ", ucTscData, iTscDataLen);
        Sleep(1000);
        return(SUCCEED);
    }


    //設定 data_0x30 內容 (用 HWEDC_DataFormat1 結構變數塞資料太累,故還是用 sprintf 方式塞資料)
    sprintf_s(data_0x30, "%c%-6.6s%-8.6s%-8.6s%-6.6s%-4s%010ld00  %-10s%08ld%015ld%03ld%08ld%-12s%-30s%s%-30s%-70s",
        0x30,                               // 訊息代碼        固定為 CHAR(30)
        sTenCode.c_str(),                   // 店號            右靠左補零
        sEcrNo.c_str(),                     // 收銀機代號      右靠左補零
        sTranNo.c_str(),                    // 收銀機交易序號
        sSalesNo.c_str(),                   // 收銀員代碼      左靠右補空白
        "A090",                             // 交易代碼
        0,                                  // 交易金額        整數 10 , 小數 2
        sInvoNo.c_str(),                    // 發票號碼
        0,                                  // 刷卡機端末代號  右靠左補零
        0,                                  // 刷卡機商店代號  右靠左補零
        3,                                  // 刷卡機 Timeout  秒
        0,                                  // 交易序號
        "",                                 // 交易參考號碼    (訂單編號)
        "",                                 // 卡片卡號        補空白
        " ",                                // 刷卡機狀態      補空白
        "",                                 // 業務需求欄位
        "Y"                                  // 特別欄位
    );

    //logsprintf("Fun_HighWayEDC_A090():data_0x30(%s), DataLen=234", logStr2Hex(send_data, 234).c_str());
   
    //sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3+1
    //sEdcSet.TscComReqTimeOut;   每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms
    iRtn=HighWayEDCComm(data_0x30, 234, recv_data, &i, sEdcSet.TsccEnqIntervalSec);
    SetHostResponseCode(iRtn);
    //送出命令 & 做通訊第一階段判斷
    if (iRtn == SUCCEED)
    {
        iTscDataLen = 226;
        memset(ucTscData, '0', iTscDataLen);
        logsprintf("Fun_HighWayEDC_A090():FETC 結帳完成");
    }
    else
    {
        logsprintf("Fun_HighWayEDC_A090:FETC 結帳 -> (通訊第一階段失敗0x30) ResponseCode=%s, rtn=%d", ucHostResponseCode, iRtn);
     }
      
    return(iRtn);    // SUCCEED

}

/**************************************************************************************************************************************************/
////////////////////////////////////////////////   此  HighWay電簽 啟動 訊息用  限台新信用卡  ////////////////////////////////////////////////


int __fastcall cLsEDC::Fun_HighWayEDC_Sig(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData)
{
    int iRtn = 0;
    string  sSP,sIN, s;
    char tmpbuf[128];

    logsprintf("★ Fun_HighWayEDC_Sig: Start 電簽啟動訊息 TranType=%d, DongleApiInputData=%s, iAddValue=%d. ★", iTranType, lpInData, iAddValue);
   
    sIN = string(lpInData);
    /*****
    if (sIN != "S")
    {
        iRtn = DATA_ERROR;
        SetHostResponseCode(iRtn);
        //錯誤處理
        sSP = "";
        RtnLogMsg("Fun_HighWayEDC_Sig:信用卡 DATA_ERROR (通訊失敗.. 電文!=S.)");
        iTscDataLen = 0;
        memset(ucTscData, 0, sizeof(ucTscData));
        return(iRtn);
    }
    ******/
    if (TranModeSw == 1)
    {
        sSP = TRANSEDCSIGFILE;
        iTscDataLen = sSP.length();
        memcpy(ucTscData, sSP.c_str(), iTscDataLen);
        logsprintf("Fun_HighWayEDC_Sig:TranMode(%s),TscDataLen(%d) ", ucTscData, iTscDataLen);
        Sleep(1000);
        return(0);
    }

    //sSP = sIN.substr(0, 1);   //bcb 1,1
    iRtn = HighWayEDC_Sig(iTranType, lpInData, iAddValue);

    if (iRtn < 0)
         iRtn = iRtn * (-1);
    SetHostResponseCode(iRtn);
   
    // 電簽 OK
    if (iRtn == SUCCEED)
    {
        sSP = EDCSIGFILE;
        iTscDataLen = sSP.length();
        memcpy(ucTscData, sSP.c_str(), sSP.length());
        logsprintf("Fun_HighWayEDC_Sig: Succeed ==(%s),TscDataLen(%d) ", ucTscData, iTscDataLen);
       
    }
    else
    {
        sSP = "";
        iTscDataLen = 0;
        memset(ucTscData, 0, sizeof(ucTscData));
        logsprintf("Fun_HighWayEDC_Sig: Error result=(%d , 0X%04X )", iRtn, iRtn);
        
    }

    return(iRtn);
}

/**************************************************************************************************************************************************/

/**************************************************************************************************************************************************/
int __fastcall cLsEDC::HighWayEDC_Sig(int iTranType, LPSTR ptr, int iCash)
{
    int iRtn=0;
    string s;
    
    if (PathFileExists( _StringToWString(EDCSIGFILE).c_str() )  )    //電簽檔
    {
        DeleteFile(_StringToWString(EDCSIGFILE).c_str());
        logsprintf("HighWayEDC_Sig: Delete file=%s ", EDCSIGFILE);
    }

    if (PathFileExists(_StringToWString(EDCSIGFILETMP).c_str()))    //電簽檔TMP
    {
        DeleteFile(_StringToWString(EDCSIGFILETMP).c_str());
        logsprintf("HighWayEDC_Sig: Delete file=%s ", EDCSIGFILETMP);
    }

    //iTscX42TimeOut =35
    iRtn = HighWayEDC_41_Sig(ptr, iCash);   // Dont close com port

    /*************************************************************************************************/
    if (iRtn == SUCCEED)
    {
      iRtn = HighWayEDC_42Data(0, iTscX42TimeOut);
        SetHostResponseCode(iRtn);
        if (iRtn == 0)
        {
            CopyFile(_StringToWString(EDCSIGFILETMP).c_str() , _StringToWString(EDCSIGFILE).c_str() , FALSE);
            logsprintf("HighWayEDC_Sig: CopyFile %s To %s ", EDCSIGFILETMP, EDCSIGFILE);
         }

    }  // end of  if( result == 0)
    /*************************************************************************************************/

    logsprintf("HighWayEDC_Sig: iRtn=%d ", iRtn);
    return(iRtn);
}


/**************************************************************************************************************************************************/


int __fastcall cLsEDC::HighWayEDC_41_Sig(LPSTR ptr, int iCash)
{

     //通訊 刷卡機與收銀機連線規格V2_3.pdf
     //
     // POS (0X41)------------------> EDC      //7.2 電簽 啟動 訊息
     //
     // POS <------------------(0X41) EDC
     //     TimeOut 180 Sec
     // POS <------------------(0X42) EDC
     // POS (0X42)------------------> EDC
     // POS <------------------(0X42) EDC
     // POS (0X42)------------------> EDC
     // .......


     // Clear ComPort Data
    int rec_len,iRtn, SendDataLen;
    int  iStatus;
    string s;
    char SrData[1024], LenBCD[3], data_0x41[128], recv_data[1024];

    memset(SrData, 0, sizeof(SrData));
    iRtn = DATA_ERROR;    //"讀寫器錯誤!!";iRtn = DATA_ERROR;
               
    sprintf_s(SrData, "%s", ptr);

    // 0x41電簽 啟動 訊息
    data_0x41[0] = 0x41;  // MsgId D65 = H41
    data_0x41[1] = 0x00;  // Len 00 01
    data_0x41[2] = 0x01;
    data_0x41[3] = SrData[0]; //// ActionCode 'S' 進行電子簽名 , 'P' 進行紙本簽名
    data_0x41[4] = 0;
    SendDataLen = 4; //<MsgId>{MsgInfo}

    //送出命令 
    //sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3+1
    //sEdcSet.TscComReqTimeOut;   每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms
    iRtn = HighWayEDCComm(data_0x41, SendDataLen, recv_data, &rec_len, sEdcSet.TsccEnqIntervalSec);
    //SetHostResponseCode(iRtn);

    if (iRtn == SUCCEED)
    {
        memcpy(&SrData, recv_data, rec_len);
        iTscDataLen = rec_len;
        iStatus = BcdToInt(&SrData[1]);
        logsprintf("HighWayEDC_41_Sig:HighWayEDC_Send<-(結束接收)(%s)  iStatus=%d", logStr2Hex(SrData, rec_len).c_str(), iStatus);
        //iRtn = iStatus;
    }
    else
    {
        logsprintf("HighWayEDC_41_Sig: (通訊失敗0x41) ResponseCode=%s, rtn=%d", ucHostResponseCode, iRtn);
        //iRtn = DATA_ERROR;
    }
     
    
    SetHostResponseCode(iRtn);

    logsprintf("HighWayEDC_41_Sig:(%s .. iRtn=%d)", iRtn == 0 ? " 電簽啟動成功" : "電簽啟動失敗", iRtn);
    return(iRtn);

}

/**************************************************************************************************************************************************/
/**************************************************************************************************************************************************/
int __fastcall cLsEDC::RepEDC_42(LPSTR ptr, int iLen)
{
    //DownLoadInfo  ==  ptr;
    //教育訓練模式判斷

     //通訊 刷卡機與收銀機連線規格V2_3.pdf
     //
     // POS (0X42)------------------> EDC
     // .......

    //sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3000+1000
    //sEdcSet.TscComReqTimeOut;   每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms
   
    int rt1 = sEdcSet.TsccRtyConnectCnt; // 連線重試次數  3; //rt = ReTry 次數
    int t1 = sEdcSet.TsccEnqIntervalSec;  ////sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3000+1000

    int result, isend_data;

    string s;
    //bool LRC_Error_0x41;
    result = EDC_ERROR;     

     ///////////////////////////////
    char send_data[256], LRC[3], cBCD[3], X42data[13];
    DWORD dwBytesWritten = 0;


    //logsprintf("RepEDC_42: ptr_data(%s), DataLen=%d", logStr2Hex(ptr, iLen).c_str(), iLen);
    //TsccTraceLog(s);
    
    IntToBcd(iLen, cBCD);
    cBCD[2] = 0;

    memset(X42data, 0x00, sizeof(X42data));
    X42data[0] = 0x42;
    X42data[1] = cBCD[0];
    X42data[2] = cBCD[1];
    memcpy(&X42data[3], ptr, iLen);

    memcpy(LRC, LRC_VALUE(X42data, iLen + 3).c_str(), 2);
    LRC[2] = 0;

    memset(send_data, 0x00, sizeof(send_data));
    send_data[0] = STX;
    memcpy(&send_data[1], X42data, iLen + 3);
    send_data[iLen + 4] = ETX;
    memcpy(&send_data[iLen + 5], LRC, 2);
    send_data[iLen + 7] = 0;
    isend_data = iLen + 7;

    ///////////////////////
    //for (i=1; i<=rt1 ; i++)   //rt1 = ReTry 次數
    //     {
    //        if (LRC_Error_0x41)  // LRC Error
    //           {
    //             SrData[0]=NAK;
    //             ComTransmitData(send_data, iLen+4 );
    //             s.sprintf("RepEDC_42(%ld) LRC_Error_0x41, HighWayEDC_Send->(NAK)", i);
    //             RtnLogMsg(s);

    //           }
    //        else
    //           {

    //logsprintf("RepEDC_42: send_data(%s), DataLen=%d", logStr2Hex(send_data, isend_data).c_str(), isend_data);
    //TsccTraceLog(s);
    
    dwBytesWritten=SCOMx.Write(send_data, isend_data);    //Send data to EDC  ComTransmitData(send_data, isend_data);
    logsprintf("RepEDC_42: send_data(%s), DataLen=%d , BytesWritten=%d", 
         logStr2Hex(send_data, isend_data).c_str(), isend_data, dwBytesWritten);
  
    result = SUCCEED;

    //           }
    //     }

    return(result);

}
/**************************************************************************************************************************************************/

/**************************************************************************************************************************************************/

//ref: 刷卡機與收銀機連線規格V2_3.pdf  7.5 DownloadInfo 說明
int __fastcall cLsEDC::HighWayEDC_42_ToFile(LPSTR ptr, int iDataLen, LPSTR pRepDownLoadInfo, int& iRe_SendCount, int& iRecCount)
{
    // ptr  = X42Data
    HWEDC_Data_0x42 sHWEDC_Data_0x42;
    DownLoadInfo  sDownLoadInfo;

    int rtn;
    int iTotalCount, iSendCount, iReceiveCount;
    string s;

    iTotalCount = iSendCount = iReceiveCount = 0;

    memset(&sHWEDC_Data_0x42, 0x00, sizeof(HWEDC_Data_0x42));
    memcpy(&sHWEDC_Data_0x42, ptr, iDataLen);
    rtn = BcdToInt(&sHWEDC_Data_0x42.cStatus[0]);
    if (rtn != 0)
    {
        logsprintf("HighWayEDC_42_ToFile: sHWEDC_Data_0x42.cStatus (%d) Receive Error Code.. ", rtn);
        //TsccTraceLog(s);
        // delete file;
        return(rtn * (-1));     // < 0 for Status error code
    }

    //char ctmp[128];
    //memset(ctmp, 0x00, sizeof(ctmp) );
    memcpy(&sDownLoadInfo, &sHWEDC_Data_0x42.sDownLoadInfo, sizeof(DownLoadInfo));
    //memcpy(ctmp, &sHWEDC_Data_0x42.sDownLoadInfo , sizeof(DownLoadInfo) );

    iTotalCount = BcdToInt(&sDownLoadInfo.cTotalCount[0]);
    iSendCount = BcdToInt(&sDownLoadInfo.cSendCount[0]);
    iReceiveCount = BcdToInt(&sDownLoadInfo.cReceiveCount[0]);

    logsprintf("HighWayEDC_42_ToFile: Rcv DownLoadInfo (%s) => iTotalCount=%d, iSendCount=%d, iReceiveCount=%d ",
        logStr2Hex((char*)&sDownLoadInfo, 6).c_str(),   iTotalCount, iSendCount, iReceiveCount);
    //TsccTraceLog(s);


    ///////////////////   Save to File
    int  iFileSize = BcdToInt(&sHWEDC_Data_0x42.cFileSize[0]);
    int  iBlockLen = BcdToInt(&sHWEDC_Data_0x42.cBlockLen[0]);
    // iFileSiz >= (iBlockLen : every uBlockData Len )

    logsprintf("SaveFile:%s ,FileSize=%d==> uBlockData(%s)-iBlockLen(%d) ", EDCSIGFILETMP, iFileSize,
        logStr2Hex(sHWEDC_Data_0x42.uBlockData, iBlockLen).c_str(), iBlockLen   );
    //TsccTraceLog(s);
    

    WriteDllFile32(EDCSIGFILETMP, sHWEDC_Data_0x42.uBlockData, iBlockLen);
    iReceiveCount++;
    //if( iReceiveCount==iSendCount)   Receive  uBlockData.SendCount OK)
    rtn = iTotalCount - iReceiveCount;     // 0: File receive OK
    iRe_SendCount = 0;                   //0: Not to Resend.
    iRecCount = iReceiveCount;
    IntToBcd(iReceiveCount, &sDownLoadInfo.cReceiveCount[0]);
    memcpy(pRepDownLoadInfo, &sDownLoadInfo, sizeof(DownLoadInfo));

    //sDownLoadInfo.iReceiveCount++;
    //ref: 刷卡機與收銀機連線規格V2_3.pdf  7.5 DownloadInfo 說明
    //rtn=sDownLoadInfo.iTotalCount - sDownLoadInfo.iSendCount;   // 0: File receive OK

    logsprintf("HighWayEDC_42_ToFile: Send DownLoadInfo :RepDownLoadInfo(%s), iTotalCount(%d) iSendCount(%d), iReceiveCount(%d) ==> (iTotalCount-iReceiveCount)=rtn=%d ",
        logStr2Hex(pRepDownLoadInfo, sizeof(DownLoadInfo)).c_str(), iTotalCount, iSendCount, iReceiveCount, rtn);
    //TsccTraceLog(s);

    return(rtn);

}
/**************************************************************************************************************************************************/

/**************************************************************************************************************************************************/

int __fastcall cLsEDC::HighWayEDC_42Data(int ComTimeOut, int WaitSec42TimeOut)
{
    //string tmp;
    int AvailableBytes = 0;
    int ReadBytes = 0;
    int iucharenqdateLen;  
    DownLoadInfo sDownLoadInfo;
    //char utmpcharenqdate[MAX_PACK_DATA_SIZE + 1], ucharenqdate[MAX_PACK_DATA_SIZE + 1];
    char *utmpcharenqdate, *ucharenqdate;

    utmpcharenqdate = (char*)malloc(MAX_PACK_DATA_SIZE + 1);
    ucharenqdate = (char*)malloc(MAX_PACK_DATA_SIZE + 1);

    memset(ucharenqdate, 0, MAX_PACK_DATA_SIZE + 1);
    memset(utmpcharenqdate, 0, MAX_PACK_DATA_SIZE + 1);

    int itimeoutcnt, itimeoutcnt42, iETXOK, icnt, rtn, iRe_SendCount, iRecCount, iStatus;
    icnt = iETXOK = 0;
    rtn = -1;

    //sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3000+1000
    //sEdcSet.TscComReqTimeOut;   每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms

    if (ComTimeOut == 0)
        ComTimeOut = sEdcSet.TsccEnqIntervalSec;

    if (WaitSec42TimeOut == 0)
        WaitSec42TimeOut = sEdcSet.TscComReqTimeOut;  //TscComReqTimeOut:每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms

    
    itimeoutcnt =  sEdcSet.TscComReceiveInterval;   //TscComReceiveInterval: 每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2)) 之偵測 Interval default:3ms
    itimeoutcnt42 = WaitSec42TimeOut / itimeoutcnt;

    logsprintf("--Start Dongle_Read_42Data %s--Start Read Data Set ComTimeOut=%d , WaitSec42TimeOut=%d ",  EDCSIGFILE, ComTimeOut, WaitSec42TimeOut);
      
     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    icnt = 0;
    iETXOK = 0;
    iRecCount = 0;
    iRe_SendCount = 0;
    ReadBytes = 0;
    iucharenqdateLen = 0;

    while (1)
    {
        if (ComConnected == true)
        {
            icnt++;
            Sleep(itimeoutcnt);
            AvailableBytes = SCOMx.BytesWaiting();
            if (AvailableBytes > 0)
            {
                memset(utmpcharenqdate, 0, sizeof(utmpcharenqdate));
                ReadBytes = SCOMx.Read((char*)utmpcharenqdate, MAX_PACK_DATA_SIZE);  // AvailableBytes);   //注意AvailableBytes,會漏接

                if (ReadBytes > 0)
                {
                    if ( (iucharenqdateLen + ReadBytes) < MAX_PACK_DATA_SIZE)
                    {
                        logsprintf("ReadFile:(%s)(%d)", logStr2Hex(utmpcharenqdate, ReadBytes).c_str(), ReadBytes);
                        //TsccTraceLog(tmp);
                        memcpy(&ucharenqdate[iucharenqdateLen], utmpcharenqdate, ReadBytes);
                        iucharenqdateLen += ReadBytes;

                        //STX(1)+ DATA(226)+ ETX(1)+ LRC(2)
                        if (ucharenqdate[iucharenqdateLen - 3] == ETX)
                        {
                            iETXOK = 1;
                        }

                    }//if(ReadBytes>0)
                    else
                    {
                        iucharenqdateLen += ReadBytes;
                    }

                }//if(ReadFile(,,,)

                if (icnt > itimeoutcnt42 || iucharenqdateLen > MAX_PACK_DATA_SIZE)
                {
                    rtn=DATA_ERROR;
                    logsprintf("Dongle_Read_42Data: Rcv DAta Memory overfloat or TimeOut ..... icnt=%d , itimeoutcnt42=%d, iucharenqdateLen=%d",
                               icnt, itimeoutcnt42, iucharenqdateLen);
                    //TsccTraceLog(tmp);
                    break;
                }


                if (iETXOK == 1)  //Receive ETX PackData
                {
                    //iRe_SendCount=0;                   //0: Not to Resend.
                    //RecCount                           Receive OK
                    memset(utmpcharenqdate, 0, sizeof(utmpcharenqdate));      // for Rep utmpcharenqdate   DownLoadInfo
                    rtn = HighWayEDC_42_ToFile(&ucharenqdate[1], iucharenqdateLen - 4, utmpcharenqdate, iRe_SendCount, iRecCount);
                    RepEDC_42(utmpcharenqdate, sizeof(DownLoadInfo));
                    iucharenqdateLen = 0;
                    //ref: 刷卡機與收銀機連線規格V2_3.pdf  7.5 DownloadInfo 說明
                    //rtn=sDownLoadInfo.iTotalCount - sDownLoadInfo.iSendCount;   // 0: File receive OK
                    memset(ucharenqdate, 0, sizeof(ucharenqdate));
                    iETXOK = 0;

                    if (rtn <= 0)  // rtn > 0 表示EDC還有資料要收.
                    {
                        if (rtn == 0)
                            logsprintf("--END Dongle_Read_42Data: %s File Rcv OK  --", EDCSIGFILETMP);
                        else
                            logsprintf("--END Dongle_Read_42Data: %s File Rcv Error Status(%d)  --", EDCSIGFILETMP, rtn);
                        //TsccTraceLog(tmp);
                       
                        break;
                    }
                }
                else if (ucharenqdate[0] == NAK || ucharenqdate[1] == NAK)
                {

                    rtn = NAK;
                    logsprintf("--END Dongle_Read_42Data: %s File Rcv Error Status(NAK)  --", EDCSIGFILE);
                    //RtnLogMsg(tmp);
                    break;
                }
                else
                {
                    ;;
                }

            }//if(AvailableBytes>0)
                     
        }//if( Connected==true)
        else
        {
            icnt = 9999;
            iucharenqdateLen = 0;
            logsprintf("--Dongle_Read_42Data: ComConnected=False,, Com Port Error -- ");
            break;
        }
    } // end of while

  
    free(utmpcharenqdate);  
    free(ucharenqdate);  
    //SetHostResponseCode(rtn);
    return(rtn);

}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
//7.2 電簽確認訊息  x43
int __fastcall cLsEDC::Fun_HighWayEDC_Confirm(int iTranType, int iTranModeSw, int iAddValue, LPSTR lpInData)
{
    int result = -1;
    string  sSP, s;


    logsprintf("★ Fun_HighWayEDC_Confirm: Start 電簽確認訊息 TranType=%d, iAddValue=%d. ★ ", iTranType, iAddValue);
    //RtnLogMsg(s);

    if (TranModeSw == 1)
    {
        sSP = TRANSEDCSIGFILE;
        iTscDataLen = sSP.length();
        memcpy(ucTscData, sSP.c_str(), iTscDataLen);
        logsprintf("Fun_HighWayEDC_Confirm:TranMode(%s),TscDataLen(%d) ", ucTscData, iTscDataLen);
        Sleep(1000);
        return(SUCCEED);
    }
    s = string(lpInData);
    sSP = s.substr(0, 1);
    result = HighWayEDC_43_Confirm((LPSTR)sSP.c_str(), iAddValue);

    // 電簽 OK
    if (result == 0)
    {
        sSP = EDCSIGFILE;
        iTscDataLen = sSP.length();
        memcpy(ucTscData, sSP.c_str(), sSP.length());
        logsprintf("Fun_HighWayEDC_Confirm: Succeed ==(%s),TscDataLen(%d) ", ucTscData, iTscDataLen);
     }

    return(result);

}




/**************************************************************************************************************************************************/

int __fastcall cLsEDC::HighWayEDC_43_Confirm(LPSTR ptr, int iCash)
{

    //教育訓練模式判斷

     //通訊 刷卡機與收銀機連線規格V2_3.pdf
     //
     // POS (0X43)------------------> EDC      //7.2 電簽確認訊息
     //       TimeOut 5 Sec
     // POS <------------------(0X43) EDC

     // Clear ComPort Data
        
    int rt3 = sEdcSet.TsccRtyConnectCnt; // = ReTry 次數
    int t1 = sEdcSet.TsccEnqIntervalSec; //sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3000+1000

    //sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3+1
    //sEdcSet.TscComReqTimeOut;   每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms
    int ConfirmTimeout = iTscX43ConfirmTimeout; // default 5000
    int  iStatus, recv_len, iRtn, SendDataLen, SrDataLen;  
    CHAR SrData[1024], LenBCD[3], LRC[3], data_0x43[128], send_data[239], recv_data[231];
    string s;
  
    memset(SrData, 0, sizeof(SrData));
    ///////////////////////////////
     //iStatus=1;
    sprintf_s(SrData, "%s", ptr);
        
    data_0x43[0] = 0x43;
    data_0x43[1] = 0x00;
    data_0x43[2] = 0x01;
    data_0x43[3] = SrData[0]; //'S':x53  ||'P':x50;
    data_0x43[4] = 0;
    SendDataLen = 4;

   // sprintf(data_0x43,"%c%c%c%s",
   //                  67,                               // MsgId D65 = H41
   //                  "01",                             // Len 00 01
   //                 ptr                              // ActionCode 'S' 進行電子簽名 , 'P' 進行紙本簽名
   //                   );                               // 4Byte
                                                      // 0x41電簽 確認 訊息

    // memcpy(LRC, LRC_VALUE(&data_0x43[0], 4).c_str(), 2);
    //send_data[0] = STX;
    //memcpy(&send_data[1], data_0x43, 4);
    //send_data[5] = ETX;
    //memcpy(&send_data[6], LRC, 2);
    //send_data[8] = 0;
    //SendDataLen = 8;

    logsprintf("HighWayEDC_43_Confirm:data_0x43(%s), DataLen=%d", logStr2Hex(data_0x43, SendDataLen).c_str(), SendDataLen);
    iStatus = EDC_ERROR;
    //送出命令 
    //sEdcSet.TsccEnqIntervalSec  X31 TimeOut  default 3+1
    //sEdcSet.TscComReqTimeOut;   每一Pack ( STX(1)+ DATA[]+ ETX(1)+ LRC(2))  TimeOut default:30000ms
    iRtn = HighWayEDCComm(data_0x43, SendDataLen, recv_data, &recv_len, ConfirmTimeout);
    SetHostResponseCode(iRtn);

    if (iRtn == SUCCEED)
    {
        memcpy(&SrData, recv_data, recv_len);
        iTscDataLen = recv_len;
        iStatus = BcdToInt(&SrData[1]);
        logsprintf("HighWayEDC_43_Confirm:<-(結束接收)(%s)", logStr2Hex(SrData, recv_len).c_str());
    }
    else
    {
        logsprintf("HighWayEDC_43_Confirm: (通訊失敗0x43) ResponseCode=%s, rtn=%d, x%04X", ucHostResponseCode, iRtn);
    }

    iRtn = iStatus;
    SetHostResponseCode(iRtn);

    logsprintf("HighWayEDC_43_Confirm:(%s .. iRtn=%d)", iRtn == 0 ? " 電簽確認訊息成功" : "電簽確認訊息失敗", iRtn);
    return(iRtn);


}

/**************************************************************************************************************************************************/

/**************************************************************************************************************************************************/

/**************************************************************************************************************************************************/


void __fastcall cLsEDC::EdcTraceLog(LPSTR pstr)
{
	EdcTraceLog(string(pstr));
}

void __fastcall cLsEDC::RtnLogMsg(string sLog)
{
    EdcTraceLog(sLog);
}

//void __stdcall logsprintf(const string& format, ...)
void __fastcall cLsEDC::logsprintf(const char* format, ...)
{
	char buffer[MAX_PACK_DATA_SIZE];
    //char *buffer;
    //buffer = (char*)malloc(MAX_PACK_DATA_SIZE);

	va_list argList;

	va_start(argList, format);
	vsnprintf_s(buffer, MAX_PACK_DATA_SIZE, format, argList);
	va_end(argList);
 
	EdcTraceLog(buffer);
    //free(buffer);
}


void __fastcall  cLsEDC::Debuglogsprintf(const char* format, ...)
{
    if (!DebugSw)
        return;

    char buffer[MAX_PACK_DATA_SIZE];
    va_list argList;

    va_start(argList, format);
    vsnprintf_s(buffer, MAX_PACK_DATA_SIZE, format, argList);
    va_end(argList);

    EdcTraceLog( "* "+string(buffer) );

}


void __fastcall cLsEDC::EdcTraceLog(string sLog)
{
	string str_now, stmp;

    fstream file;
	string str_path = EDCTRACEDIR;
	string str_ecr;
    str_now = _currentDateTime(0, 0); // FormatDateTime("yyyy-mm-dd hh:nn:ss", Now());

	try
	{
		str_ecr = "EDCTrac" + _currentDateTime(5, 0);
		string str_file = str_path + str_ecr + ".log";
		file.open(str_file.c_str(), ios::out | ios::app);
		if (file)
		{
			//file.open(log_name,ios::out | ios::app /*| ios::binary*/ );
			//dttm2 += " " +  string(pStr) + "\a\n";
			//dttm2.append(" ");
			//dttm2.append(string(pStr) );
			file << str_now.c_str() << " " << sLog.c_str() << "\n";
		}

	}
	catch (...)
	{
		;;
	}

	file.close();

}




int __fastcall cLsEDC::GetClint(string sSection)
{
    string Sn;
    //Sn = sSection;
    Sn = "TsccThread";
	try
	{
    	//READ TsccThread INI
		sEdcSet.TscComDeviceName = Clini.ReadString("TsccThread", "TscComDeviceName", "COM1");
		sEdcSet.TscComBaudRate = stoi(Clini.ReadString("TsccThread", "TscComBaudRate", "115200")); // .c_str() );
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
		
        iTsccReqTimeOut = stoi(Clini.ReadString("TsccThread", "TsccReqTimeOut", "35000"));     //????
        iTscX42TimeOut =  stoi(Clini.ReadString("TsccThread", "TscX42TimeOut", "35000")); 
        iTscX40TimeOut = stoi(Clini.ReadString("TsccThread", "TscX42TimeOut", "35000"));
        iTscX43ConfirmTimeout = stoi(Clini.ReadString("TsccThread", "TscX43ConfirmTimeout", "5000"));

		//READ END
		//WRITE X INI
		Clini.WriteString("TsccThread", "TscComDeviceName", sEdcSet.TscComDeviceName);
		Clini.WriteString("TsccThread", "TscComBaudRate", _int2str( sEdcSet.TscComBaudRate));
		Clini.WriteString("TsccThread", "TscComParity", _int2str(sEdcSet.TscComParity));
		Clini.WriteString("TsccThread", "TscComByteSiz", _int2str(sEdcSet.TscComByteSiz));

		Clini.WriteString("TsccThread", "TscComStopBits", _int2str(sEdcSet.TscComStopBits));
		Clini.WriteString("TsccThread", "TsccEnqIntervalSec", _int2str(sEdcSet.TsccEnqIntervalSec));
		Clini.WriteString("TsccThread", "TscComReceiveInterval", _int2str(sEdcSet.TscComReceiveInterval));
		Clini.WriteString("TsccThread", "TscComReqTimeOut", _int2str(sEdcSet.TscComReqTimeOut));
		Clini.WriteString("TsccThread", "TsccReqIntervalCnt", _int2str(sEdcSet.TsccReqIntervalCnt));

		Clini.WriteString("TsccThread", "TsccReqIntervalTime", _int2str(sEdcSet.TsccReqIntervalTime));
		Clini.WriteString("TsccThread", "TsccRtyConnectCnt", _int2str(sEdcSet.TsccRtyConnectCnt));
		Clini.WriteString("TsccThread", "TsccRtyConnectIntervalTime", _int2str(sEdcSet.TsccRtyConnectIntervalTime));
        
        Clini.WriteString("TsccThread", "TsccReqTimeOut", _int2str(iTsccReqTimeOut));
        Clini.WriteString("TsccThread", "TscX42TimeOut",  _int2str(iTscX42TimeOut));
        Clini.WriteString("TsccThread", "TscX40TimeOut", _int2str(iTscX40TimeOut));
        Clini.WriteString("TsccThread", "TscX43ConfirmTimeout", _int2str(iTscX43ConfirmTimeout));

        Debuglogsprintf("GetClint(%s) OK", Sn.c_str() );

		//WRITE END
	}
	catch (...)
	{
        logsprintf("GetClint(%s) Error...", Sn.c_str());
	}

	return SUCCEED;
}



// Common Function InitComPort()
int __fastcall cLsEDC::InitComPort()
{

    int irtn;
    string stmp;
    UCHAR cb;
        
    ComConnected = false;
    TErrCode = 0;
    sReceivedData = "";
    sTsccApiErrorMsg = "";
    irtn = 0;
    cb = 0;

    //if( TranModeSw || TsccOffLineApi)
    if (TranModeSw)
    {
        _Strprintf(stmp, "InitComPort DeviceName=Com%d,  TranModeSw=%d, Return OK", sEdcSet.iDeviceNo, TranModeSw);
        RtnLogMsg(stmp);
        return(SUCCEED);
    }

    // 設定 預設可用之通信Port
    //if (DeviceName == "")
    // string  DeviceName = "COM1";
      
    GetClint("TsccThread");   // Get Set Prm from INI
   
    OriginalDCB.DCBlength = sizeof(DCB);
    #pragma region   Set Com port BaudRate
    OriginalDCB.BaudRate = 9600; //(DWORD)BaudRate;
    switch (sEdcSet.TscComBaudRate)
    {
    case 9600:
        OriginalDCB.BaudRate = 9600;
        break;
    case 14400:
        OriginalDCB.BaudRate = 14400;
        break;
    case 19200:
        OriginalDCB.BaudRate = 19200;
        break;
    case 38400:
        OriginalDCB.BaudRate = 38400;
        break;
    case 57600:
        OriginalDCB.BaudRate = 57600;
        break;
    case 115200:
        OriginalDCB.BaudRate = 115200;
        break;
    case 128000:
        OriginalDCB.BaudRate = 128000;
        break;
    case 256000:
        OriginalDCB.BaudRate = 256000;
        break;
    defaule:
        OriginalDCB.BaudRate = 9600;
        //break;
    }
    #pragma endregion

    #pragma region   Set Com port ByteSize
    //UCHAR cb;
    OriginalDCB.ByteSize = 8;       //ByteSize; 8
    switch (sEdcSet.TscComByteSiz)    
    {
    case 7:
        OriginalDCB.ByteSize = 7;
        break;
    default:
        OriginalDCB.ByteSize = 8;
        break;
    }
    #pragma endregion

    #pragma region   Set Com port Parity
    //cb = Parity;
    OriginalDCB.Parity = NOPARITY;  //Parity;     //BYTE Parity; 0=NOPARITY, 1=ODDPARITY, 2=EVENPARITY, 3=MARKPARITY, 4=SPACEPARITY
    switch (sEdcSet.TscComParity)    
    {
    case 1:
        OriginalDCB.Parity = ODDPARITY;
        break;
    case 2:
        OriginalDCB.Parity = EVENPARITY;
        break;
    case 3:
        OriginalDCB.Parity = MARKPARITY;
        break;
    case 4:
        OriginalDCB.Parity = SPACEPARITY;
        break;
    default:
        OriginalDCB.Parity = NOPARITY;
        break;
    }
    #pragma endregion

    #pragma region   Set Com port StopBits
    //cb = StopBits;
    OriginalDCB.StopBits = ONESTOPBIT; //StopBits; // BYTE StopBits; 0=ONESTOPBIT,1=ONE5STOPBITS,2=TWOSTOPBITS => 1, 1.5, 2
    switch (sEdcSet.TscComStopBits)    
    {
    case 2:
        OriginalDCB.StopBits = TWOSTOPBITS;
        break;
    default:
        OriginalDCB.StopBits = ONESTOPBIT;
        break;
    }
    #pragma endregion

    #pragma region   Set Com port Floe control
    OriginalDCB.fBinary = false;
    OriginalDCB.fParity = false;
    OriginalDCB.fRtsControl = RTS_CONTROL_DISABLE;        //RTS_CONTROL_DISABLE;  RTS_CONTROL_ENABLE;
    OriginalDCB.fDtrControl = DTR_CONTROL_DISABLE;        //DTR_CONTROL_DISABLE;   DTR_CONTROL_ENABLE;
    OriginalDCB.fOutxCtsFlow = false;
    OriginalDCB.fOutxDsrFlow = false;
    OriginalDCB.fDsrSensitivity = false;
    OriginalDCB.fTXContinueOnXoff = false;
    OriginalDCB.fOutX = false;
    OriginalDCB.fInX = false;
    OriginalDCB.fAbortOnError = false;
    #pragma endregion

    #pragma region   Set Com port system timeout
    OriginalTimeouts.ReadIntervalTimeout = 500;
    OriginalTimeouts.ReadTotalTimeoutMultiplier = 10;
    OriginalTimeouts.ReadTotalTimeoutConstant = 500;
    OriginalTimeouts.WriteTotalTimeoutMultiplier = 10;
    OriginalTimeouts.WriteTotalTimeoutConstant = 500;
    #pragma endregion

    try
    {
        SCOMx.ComOpen(_StringToWString(sEdcSet.TscComDeviceName).c_str(), OriginalDCB, FALSE);

        

        ComConnected = true;

        // SCOMx.ComOpen( StringToWString(sEdcSet.TscComDeviceName).c_str(),
        //    OriginalDCB.BaudRate, SCOMx.NoParity,  OriginalDCB.ByteSize, SCOMx.OneStopBit  ,
        //   SCOMx.NoFlowControl, false);
        
        SCOMx.GetState(MyDCB);
        SCOMx.GetTimeouts(MyTimeouts);        // Get the current Timeouts to Backup it
        SCOMx.SetTimeouts(OriginalTimeouts);  // Set the current Timeouts to Backup it
        SCOMx.Setup(MAX_PACK_DATA_SIZE, MAX_PACK_DATA_SIZE);  // Set Com Input, output Buffer
        irtn = SUCCEED;
        //sEdcSet.iDeviceNo = StrToInt(DeviceName.SubString(4, 2).Trim());
        logsprintf("InitComPort: %s, BaudRate=%d, Parity=%d, ByteSize=%d, StopBits=%d  Succeed...",
            sEdcSet.TscComDeviceName.c_str(), OriginalDCB.BaudRate, OriginalDCB.Parity,
            OriginalDCB.ByteSize, OriginalDCB.StopBits);
    }
    catch (CSerialException& pEx)
    {
          sTsccApiErrorMsg = pEx.what();
         logsprintf("InitComPort CSerialException: %s ", pEx.what()  );
         irtn = PORT_OPENED_ERROR;
         ComConnected = false;
    }
    catch (exception& e)
    {
        sTsccApiErrorMsg = e.what();
        logsprintf("InitComPort Exception: %s.", e.what() );
        irtn = PORT_OPENED_ERROR;
        ComConnected = false;
    }
    catch (...)
    {
        logsprintf("InitComPort carch Error.. %s open NG", sEdcSet.TscComDeviceName.c_str());
         
        //iDeviceNo = 0;
        irtn = PORT_OPENED_ERROR;
        sTsccApiErrorMsg = "InitComPort carch Error ..";
        /**
        stmp.sprintf("TTscThread::InitComPort Error DeviceName=%s Set DeviceNo=%d ",
            DeviceName.c_str(), iDeviceNo);

        RtnLogMsg(stmp);
        **/
        ComConnected = false;
    }
  
    return(irtn);
}


// Common Function CloseComPort()
int __fastcall cLsEDC::CloseComPort()
{
    int irtn;
    irtn = 0;
    string stmp;

    if (TranModeSw)
    {
        _Strprintf(stmp, "CloseComPort DeviceName=Com%d,  TranModeSw=%d, Return OK", sEdcSet.iDeviceNo, TranModeSw);
        RtnLogMsg(stmp);
        return(irtn);
    }

   
    try
    {
        if (ComConnected == true)
        {
            sTsccApiErrorMsg = "";
            #pragma region   Set Backup to the current Status
            SCOMx.SetState(MyDCB);
            SCOMx.SetTimeouts(MyTimeouts);         
            #pragma endregion
            SCOMx.Close();
            irtn = 0;
            /********
            //back to original setings
            SetCommState(SCOMx.m_hComm, &OriginalDCB);
            SetCommTimeouts(SCOMx.m_hComm, &OriginalTimeouts);
            if (CloseHandle(DeviceHandle) != 0)
            {
                OriginalDCB = MyDCB;
                OriginalTimeouts = MyTimeouts;
                ComConnected = false;
            }//if
            else
            {
                SetCommState(DeviceHandle, &MyDCB);
                SetCommTimeouts(DeviceHandle, &MyTimeouts);
            }
            ************/
            logsprintf("CloseComPort:close %s OK", sEdcSet.TscComDeviceName.c_str());
            //stmp.sprintf("TTscThread::ComClose %s BaudRate=%d, ByteSize=%d, Parity=%d, StopBits=%d ",
            //    DeviceName.c_str(), MyDCB.BaudRate,
            //    MyDCB.ByteSize, MyDCB.Parity, MyDCB.StopBits);

            ComConnected = false;
           
        }
        else
        {
            logsprintf("CloseComPort: %s already close..", sEdcSet.TscComDeviceName.c_str());
        }
        

    }
    catch(...)
    {
        logsprintf("CloseComPort carch Error ..PORT_CLOSEHANDLE_ERROR ", sEdcSet.TscComDeviceName.c_str());
        irtn = PORT_CLOSEHANDLE_ERROR;
        sTsccApiErrorMsg = "CloseComPort carch Error ..PORT_CLOSEHANDLE_ERROR";
    }
  
    return(irtn);
}



string  __fastcall cLsEDC::LRC_VALUE(string AStr) //2 bytes
{
    unsigned char lrc;
    string sLRC;

    lrc = 0;
    for (int i = 1; i <= (int)AStr.length(); i++)
        lrc ^= AStr[i];
    lrc ^= ETX;

    sLRC = _IntToHex((int)lrc);

    Debuglogsprintf("LRC_VALUE()_1:lrc=%d, sLRC=%s", lrc, sLRC.c_str());

    return(sLRC);

}

string  __fastcall cLsEDC::LRC_VALUE(LPSTR ptr, int iLen)
{
    unsigned char lrc;
    string sLRC;

    lrc = 0;
    for (int i = 0; i < iLen; i++)
        lrc ^= ptr[i];

    lrc ^= ETX;
    sLRC = _capitalizeString( _IntToHex((int)lrc) );

    Debuglogsprintf("LRC_VALUE()_2:lrc=%d, sLRC=%s", lrc, sLRC.c_str());

    //s.sprintf("LRC_VALUE_C:(%s<ETX>)  lrc=x%02X , Lrc=%s", RtnLogMs_Str2Hex(ptr , iLen), lrc , sLRC );
    //TsccTraceLog(s);
    //DebugLogMsg(s);

    return(sLRC);

}

int __fastcall cLsEDC::BcdToInt(LPSTR pBcdData2)
{
    //CHAR cRevBCD[2];
    unsigned short int ic;
    string s, strHexData;
    strHexData = "";
    int bcdint;

    for (int i = 0; i < 2; i++, pBcdData2++)
    {
        ic = *pBcdData2 & 0x00FF;;
        strHexData = strHexData + _IntToHex(ic);
    }
    bcdint = _StrToInt(strHexData); 

    //cRevBCD[0]=pBcdData2[1];
    //cRevBCD[1]=pBcdData2[0];
    //cRevBCD[2]=0;
    //memcpy(&i,cRevBCD , 2);

    Debuglogsprintf("BcdToInt (%s) ==> int=%d ", strHexData.c_str(), bcdint);
    
    return (bcdint);

}

void __fastcall cLsEDC::IntToBcd(int iBcd, LPSTR pBcdData2)
{
    CHAR cRevBCD[3];
    unsigned short int ic;
    string s, strHexData, sHBCD, sLBCD;
    strHexData = "";
   

    //strHexData= _IntToHex(iBcd);   //.sprintf("%04d", iBcd);
    _Strprintf(strHexData, "%04d", iBcd);
    sHBCD = strHexData.substr(0, 2);
    sLBCD = strHexData.substr(2, 2);

    cRevBCD[0] = (int)strtol(sHBCD.c_str(), NULL, 16);
    cRevBCD[1] = (int)strtol(sLBCD.c_str(), NULL, 16);
    cRevBCD[2] = 0;

    memcpy(pBcdData2, cRevBCD, 2);
  
    Debuglogsprintf("IntToBcd (%s) ==> BCD=%s ", strHexData.c_str(), logStr2Hex(cRevBCD, 2).c_str() );
   
/****
    strHexData = "";
    for (int i = 0; i < 2; i++, pBcdData2++)
    {
        ic = *pBcdData2 & 0x00FF;;
        strHexData = strHexData + _IntToHex(ic);
    }
    Debuglogsprintf("IntToBcd (%04d) ==> BCD=%s ", iBcd, strHexData.c_str());
****/
  
    // const char *hexstring = "abcdef0";
    //int number = (int)strtol(hexstring, NULL, 16);

}







/*********************************
//FETC 結帳
int  __fastcall TTscThread::Fun_HighWayEDC_A090(int iTranType, int iAddValue) //結帳
{
    //HWEDC_DataFormat1 data_0x30; //收銀機起始訊息格式 HWEDC_DataFormat1 (長度 234, 無頭尾)
    char data_0x30[235]; //用 HWEDC_DataFormat1 結構變數塞資料太累,故還是用 sprintf 方式塞資料
    char send_data[239]; //data len = 234 + 4 = 238
    char recv_data[231]; //data len = 226 + 4 = 230
    char LRC[3];
    HWEDC_Data_0x40 data_0x40; //刷卡機交易回應訊息格式 HWEDC_DataFormat3 (長度 226, 無頭尾)
    HWEDC_Data_0x40_spec6 data_0x40_special; //刷卡機交易回應訊息格式之特別欄位格式六 (為取出餘額等資訊)
    int i = 0;
    String s;
    // if (tran_mode_sw  )  // 教育訓練 mode
    //   {
    //     Fun_HighWayEDC_A080_Tran();
    //     return(SUCCEED);
    //   }


    //初始變數
    memset(data_0x30, '\0', sizeof(data_0x30));
    memset(send_data, '\0', sizeof(send_data));
    memset(recv_data, '\0', sizeof(recv_data));
    memset(&data_0x40, ' ', sizeof(data_0x40));    // 電子發票載具   2018/01/22   Lu update
    //char tmp_buf[128];

    // DongleApiInputData=>  DongleApiInputData.sprintf("%-6s%-8s%-8s%-6s%-10s%14s%010ld00",
    //                       sTenCode,sEcrNo,sTranNo,sSalesNo,sInvoNo, dttm, );

    //sTenCode=DongleApiInputData.SubString(1,6);
    //sEcrNo=DongleApiInputData.SubString(7,8);
    //sTranNo=DongleApiInputData.SubString(15,8);
    //sSalesNo=DongleApiInputData.SubString(23,6);
    //sInvoNo=DongleApiInputData.SubString(29,10);

    sTenCode = "000000";
    sEcrNo = "00000000";
    sTranNo = "00000000";
    sSalesNo = "000000";
    sInvoNo = "0000000000";

    s = "0000";  // 2011 通訊失敗
    memcpy(ucHostResponseCode, s.c_str(), 4);
    ucHostResponseCode[4] = 0;

    //s.sprintf("Fun_HighWayEDC_A090:e通卡 結帳 DongleApiInputData=%s",
    //           DongleApiInputData.c_str()    );
    s.sprintf("Fun_HighWayEDC_A090:FETC 結帳 ");
    RtnLogMsg(s);


    if (TranModeSw == 1)
    {

        //memset(&data_0x40_special,'0',sizeof(data_0x40_special) );
        //memcpy(&data_0x40_special.field_02, "00000000", 8);  // 現金點餘額
        //memcpy(&data_0x40_special.field_03, "00000000", 8);  // 紅利點餘額

        memset(&data_0x40, '0', 226);
        //memcpy(&data_0x40.field_specil, &data_0x40_special, sizeof(data_0x40_special) );

        iTscDataLen = 226;
        memcpy(ucTscData, &data_0x40, iTscDataLen);
        s.sprintf("Fun_HighWayEDC_A090:TranMode(%s),TscDataLen(%d) ", ucTscData, iTscDataLen);
        RtnLogMsg(s);
        Sleep(2000);
        return(0);
    }


    //設定 data_0x30 內容 (用 HWEDC_DataFormat1 結構變數塞資料太累,故還是用 sprintf 方式塞資料)
    sprintf(data_0x30, "%c%-6.6s%-8.6s%-8.6s%-6.6s%-4s%010ld00  %-10s%08ld%015ld%03ld%08ld%-12s%-30s%s%-30s%-70s",
        0x30,                               // 訊息代碼        固定為 CHAR(30)
        sTenCode.c_str(),                   // 店號            右靠左補零
        sEcrNo.c_str(),                     // 收銀機代號      右靠左補零
        sTranNo.c_str(),                    // 收銀機交易序號
        sSalesNo.c_str(),                   // 收銀員代碼      左靠右補空白
        "A090",                             // 交易代碼
        0,                                  // 交易金額        整數 10 , 小數 2
        sInvoNo.c_str(),                    // 發票號碼
        0,                                  // 刷卡機端末代號  右靠左補零
        0,                                  // 刷卡機商店代號  右靠左補零
        3,                                  // 刷卡機 Timeout  秒
        0,                                  // 交易序號
        "",                                 // 交易參考號碼    (訂單編號)
        "",                                 // 卡片卡號        補空白
        " ",                                // 刷卡機狀態      補空白
        "",                                 // 業務需求欄位
        "Y"                                  // 特別欄位
    );


    strncpy(LRC, LRC_VALUE(data_0x30).c_str(), 2);
    LRC[2] = 0;

    //打包 send_data 內容
    sprintf(send_data, "%c%-234s%c%-2s", STX, data_0x30, ETX, LRC);
    send_data[238] = 0;


    s.sprintf("Fun_HighWayEDC_A090():data_0x30(%s), DataLen=238",
        RtnLogMs_Str2Hex(send_data, 238));
    DebugLogMsg(s);


    //送出命令 & 做通訊第一階段判斷
    if (HighWayEDC_Send(send_data, 0) != SUCCEED)
    {
        //錯誤處理
        s = "2011";  // 2011 通訊失敗
        memcpy(ucHostResponseCode, s.c_str(), 4);
        ucHostResponseCode[4] = 0;
        //RelClrShowPMSG(); //2005/07/05
        //show_down_scr2(" "," "," ");
        //show_down_scr2("請按[清除]鍵"," ","e通卡 加值機讀卡失敗");
        //err_code=90; crs_buzzer_on();
        s.sprintf("Fun_HighWayEDC_A090:FETC 結帳 -> (通訊第一階段失敗0x30) ResponseCode=%s, rtn=%d",
            ucHostResponseCode, 2);
        RtnLogMsg(s);
        return(2);    //"讀寫器錯誤!!"
    }


    
    //ucHostResponseCode
    iTscDataLen = 226;
    memset(ucTscData, '0', iTscDataLen);


    //Sleep(100);
    //  memset(ucharRepdate, 0x00, sizeof(ucharRepdate) );
    //  memset( ucTscData, 0x00, sizeof(ucTscData));
    //  iTscDataLen=0;



    //s.sprintf("Fun_HighWayEDC_A090():data_0x40(%s), DataLen=226",
    //           RtnLogMs_Str2Hex(ucTscData , 226)    );
    //DebugLogMsg(s);

    //成功處理
    //char rm_buf[9];
    //strncpy(rm_buf,data_0x40_special.field_02,8);  //特別格式第六種的欄位二
    //rm_buf[8]=0;

    //int remain=atoi(rm_buf);

    //特別格式第六種的欄位四
    //int icard_type=atoi(data_0x40_special.field_04); //卡片類別="1"->銀行卡(信用卡加值)  卡片類別="2"->遠通卡(現金加值)

    s.sprintf("Fun_HighWayEDC_A090():FETC 結帳完成");
    RtnLogMsg(s);


    return(0);    // SUCCEED

}
******************************************/

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
