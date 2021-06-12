
//TsccDongleApi.h
#include <tchar.h>

#define EDCSIGFILE   "c:\\Ftlinline\\dll\\dat\\EdcSig.gz"
#define EDCSIGFILETMP   "c:\\Ftlinline\\dll\\dat\\EdcSig.tmp"
#define TRANSEDCSIGFILE   "c:\\Ftlinline\\dll\\dat\\TransEdcSig.gz"
#define EDCTRACEDIR       "c:\\Ftlinline\\log\\"

#define SUCCEED         0
#define FAILURE        -1
#define EDCTIMEOUT     -99

#define STX     0x02
#define ETX     0x03
#define ACK     0x06
#define NAK     0x15
#define CR      0x0D
#define LF      0x0A
#define ESC     0x1B

//#define INLINE_WORK_PATH   "c:\\abacus\\"
//#define MWR_SETLOCALTIME       WM_APP+3153

// #define MAX_PAGE_REC  9
//typedef unsigned char   uchar;
//typedef void __fastcall (__closure *TNotifyEventError)(System::TObject* Sender, AnsiString sStrMsg, int iErrNo);

/**                     特別注意 TCHAR=world : 2Byte  , 要用 char  || CHAR                  ****/


typedef struct //收銀機起始訊息格式 HWEDC_Data_0x30;
{
  CHAR    field_01[1];       // 訊息代碼        固定為 CHAR(30)
  CHAR    field_02[6];       // 店號            右靠左補零
  CHAR    field_03[8];       // 收銀機代號      右靠左補零
  CHAR    field_04[8];       // 收銀機交易序號
  CHAR    field_05[6];       // 收銀員代碼      左靠右補空白
  CHAR    field_06[4];       // 交易代碼
  CHAR    field_07[12];      // 交易金額        整數 10 , 小數 2
  CHAR    field_08[12];      // 發票號碼        2 位空白 + 10位發票號碼
  CHAR    field_09[8];       // 刷卡機端末代號  右靠左補零
  CHAR    field_10[15];      // 刷卡機商店代號  右靠左補零
  CHAR    field_11[3];       // 刷卡機 Timeout 秒
  CHAR    field_12[8];       // 交易序號 / 刷卡機調閱編號 右靠左補零
  CHAR    field_13[12];      // 交易參考號碼(訂單編號)
  CHAR    field_14[30];      // 卡片卡號   補空白
  CHAR    field_15[1];       // 刷卡機狀態 補空白
  CHAR    field_salereq[30]; // 業務需求欄位
  CHAR    field_specil[100]; // 特別欄位
} HWEDC_Data_0x30; //總長度 234


typedef struct //收銀機起始訊息格式 "業務需求欄位" 展開之 1
{
  CHAR    field_01[2];       // 業務代號 "01"
  CHAR    field_02[8];       // 收銀機入帳日期 yyyymmdd
  CHAR    field_03[20];      // 保留欄位 填空白
} HWEDC_Data_0x30_salreq1; //總長度 30


typedef struct //收銀機起始訊息格式 "業務需求欄位" 展開之 2
{
  CHAR    field_01[2];       // 業務代號 "10"
  CHAR    field_02[20];      // 依對帳需求各自定義
  CHAR    field_03[8];       // 保留欄位 填空白
} HWEDC_Data_0x30_salreq2; //總長度 30


typedef struct //收銀機起始訊息格式 "特別欄位" 展開之 1 (油摺卡消費交易)
{
  CHAR    field_01[12];      // 抵扣後金額      整數 10 , 小數 2
  CHAR    field_02[6];       // 油品數量        整數 4 , 小數 2
  CHAR    field_03[10];      // 油品類別        左靠右補空白
  CHAR    field_04[10];      // 車號            左靠右補空白
  CHAR    field_05[6];       // 里程數          右靠左補零
  CHAR    field_06[6];       // 客戶欄位        左靠右補空白
  CHAR    field_07[20];      // 保留欄位        空白
} HWEDC_Data_0x30_spec1; //總長度 70


typedef struct //收銀機起始訊息格式 "特別欄位" 展開之 2 (結帳交易)
{
  CHAR    field_01[1];       // 異常報表列印    'Y'=加印 , 'N'=不加印
  CHAR    field_02[69];      // 保留欄位        空白
} HWEDC_Data_0x30_spec2; //總長度 70

//---------------------------------------------------------------------------
/**   特別注意 TCHAR : Byte
typedef struct //刷卡機起始訊息格式 HWEDC_Data_0x31;
{
  TCHAR    field_01[1];       // 訊息代碼        固定為 CHAR(31)
  TCHAR    field_02[6];       // 店號            右靠左補零
  TCHAR    field_03[8];       // 收銀機代號      右靠左補零
  TCHAR    field_04[8];       // 收銀機交易序號
  TCHAR    field_05[6];       // 收銀員代碼      左靠右補空白
  TCHAR    field_06[4];       // 交易代碼
  TCHAR    field_07[12];      // 交易金額        整數 10 , 小數 2
  TCHAR    field_08[12];      // 發票號碼
  TCHAR    field_09[8];       // 刷卡機端末代號  右靠左補零
  TCHAR    field_10[15];      // 刷卡機商店代號  右靠左補零
  TCHAR    field_11[3];       // 刷卡機 Timeout  秒
  TCHAR    field_12[8];       // 交易序號        右靠左補零
  TCHAR    field_13[12];      // 交易參考號碼    (訂單編號)
  TCHAR    field_14[30];      // 卡號            左靠右補空白
  TCHAR    field_15[1];       // 刷卡機狀態      '0'=卡片已插入 , '1'=無卡片,刷卡機等待中
  TCHAR    field_16[30];      // 業務需求欄位
  TCHAR    field_specil[70];  // 特別欄位
} HWEDC_Data_0x31;  //總長度 468
**/

typedef struct //刷卡機起始訊息格式 HWEDC_Data_0x31;
{
    char    field_01[1];       // 訊息代碼        固定為 CHAR(31)
    char    field_02[6];       // 店號            右靠左補零
    char    field_03[8];       // 收銀機代號      右靠左補零
    char    field_04[8];       // 收銀機交易序號
    char    field_05[6];       // 收銀員代碼      左靠右補空白
    char    field_06[4];       // 交易代碼
    char    field_07[12];      // 交易金額        整數 10 , 小數 2
    char    field_08[12];      // 發票號碼
    char    field_09[8];       // 刷卡機端末代號  右靠左補零
    char    field_10[15];      // 刷卡機商店代號  右靠左補零
    char    field_11[3];       // 刷卡機 Timeout  秒
    char    field_12[8];       // 交易序號        右靠左補零
    char    field_13[12];      // 交易參考號碼    (訂單編號)
    char    field_14[30];      // 卡號            左靠右補空白
    char    field_15[1];       // 刷卡機狀態      '0'=卡片已插入 , '1'=無卡片,刷卡機等待中
    char    field_16[30];      // 業務需求欄位
    char    field_specil[70];  // 特別欄位
} HWEDC_Data_0x31;  //總長度 234



typedef struct //收銀機起始訊息格式 "業務需求欄位" 展開之 1
{
  CHAR    field_01[2];       // 業務代號 "01"
  CHAR    field_02[8];       // 收銀機入帳日期 yyyymmdd
  CHAR    field_03[20];      // 保留欄位 填空白
} HWEDC_Data_0x31_salreq1; //總長度 30


typedef struct //收銀機起始訊息格式 "業務需求欄位" 展開之 2
{
  CHAR    field_01[2];       // 業務代號 "10"
  CHAR    field_02[20];      // 依對帳需求各自定義
  CHAR    field_03[8];       // 保留欄位 填空白
} HWEDC_Data_0x31_salreq2; //總長度 30


typedef struct //刷卡機起始訊息格式特別欄位展開之 1 (油摺卡消費交易)
{
  CHAR    field_01[12];      // 抵扣後金額      整數 10 , 小數 2
  CHAR    field_02[6];       // 油品數量        整數 4 , 小數 2
  CHAR    field_03[10];      // 油品類別        左靠右補空白
  CHAR    field_04[10];      // 車號            左靠右補空白
  CHAR    field_05[6];       // 里程數          右靠左補零
  CHAR    field_06[6];       // 客戶欄位        左靠右補空白
  CHAR    field_07[20];      // 保留欄位        空白
} HWEDC_Data_0x31_spec1; //總長度 70


typedef struct //刷卡機起始訊息格式特別欄位展開之 2 (結帳交易)
{
  TCHAR    field_01[1];       // 異常報表列印    'Y'=加印 , 'N'=不加印
  TCHAR    field_02[69];      // 保留欄位        空白
} HWEDC_Data_0x31_spec2; //總長度 70


//---------------------------------------------------------------------------
typedef struct //刷卡機交易回應訊息格式特別欄位展開之 for FETC X50
{
  CHAR    field_01[1];       // 卡片類別        左靠右補空白
  CHAR    field_02[20];      // 卡片卡號
  CHAR    field_03[8];       // 有效日期
  CHAR    field_04[6];       // 刷卡機調閱編號  右靠左補零
  CHAR    field_05[6];       // 刷卡機批次號
  CHAR    field_06[10];      // 實際交易金額    原始交易金額    整數 10 , //小數 2
  CHAR    field_07[10];      // 交易後餘額      實際交易金額    整數 10 , //小數 2
  CHAR    field_08[12];      // 交易參考號碼
  CHAR    field_09[6];       // 交易授權碼
  CHAR    field_10[10];      // 交易前餘額      實際交易金額    整數 10 , //小數 2
  CHAR    field_11[8];       // 驗證卡編號
  CHAR    field_12[10];      // 卡片序號
  CHAR    field_13[8];       // 卡片交易序號
  CHAR    field_14[8];       // 卡片交易時間
  CHAR    field_15[8];       // 卡片扣款序號
  CHAR    field_16[8];       // 加值機驗證編號
  CHAR    field_17[6];       // 加值機驗證序號
  CHAR    field_18[8];       // 扣款驗證編號
  CHAR    field_19[8];       // 扣款驗證序號
  CHAR    field_20[16];      // 扣款亂數驗證碼
  CHAR    field_21[16];      // 遠通加值驗證碼
  CHAR    field_22[16];      // IC卡扣款驗證碼
  CHAR    field_23[1];       // Y 表示此交易做AUTOLOAD
  CHAR    field_24[16];      // 保留欄位        空白
} HWEDC_Data_0x50_FETCspec;  // 總長度 226


typedef struct //刷卡機交易回應訊息格式特別欄位展開之 for TSCC X50
{
  CHAR    field_01[1];       // 卡片類別        左靠右補空白
  CHAR    field_02[20];      // 卡片卡號
  CHAR    field_03[8];       // 有效日期
  CHAR    field_04[6];       // 刷卡機調閱編號  右靠左補零
  CHAR    field_05[6];       // 刷卡機批次號
  CHAR    field_06[10];      // 實際交易金額    原始交易金額    整數 10 , //小數 2
  CHAR    field_07[10];      // 交易後餘額      實際交易金額    整數 10 , //小數 2
  CHAR    field_08[12];      // 交易參考號碼
  CHAR    field_09[6];       // 交易授權碼
  CHAR    field_10[10];      // 交易前餘額      實際交易金額    整數 10 , //小數 2
  CHAR    field_11[8];       // 交易前累計加值  實際交易金額    整數 8 , //小數 2
  CHAR    field_12[8];       // 交易前累計扣款  實際交易金額    整數 8 , //小數 2
  CHAR    field_13[8];       // 驗證卡編號
  CHAR    field_14[10];      // 卡片序號
  CHAR    field_15[8];       // 卡片交易序號
  CHAR    field_16[8];       // 交易前卡片扣款序號
  CHAR    field_17[8];       // 扣款驗證序號
  CHAR    field_18[16];      // 扣款亂數驗證碼
  CHAR    field_19[16];      // 扣款/加值驗證碼
  CHAR    field_20[8];       // 交易前卡片加值序號
  CHAR    field_21[8];       // 發卡銀行代號
  CHAR    field_22[1];       // Y 表示此交易做AUTOLOAD
  CHAR    field_23[30];      // 保留欄位        空白
} HWEDC_Data_0x50_TSCCspec;  // 總長度 226


typedef struct //刷卡機交易回應訊息格式 HWEDC_Data_0x50;
{
  CHAR    field_01[1];       // 訊息代碼        固定為 CHAR(40)
  CHAR    field_02[6];       // 店號            右靠左補零
  CHAR    field_03[8];       // 收銀機代號      右靠左補零
  CHAR    field_04[8];       // 收銀機交易序號
  CHAR    field_05[6];       // 收銀員代碼      左靠右補空白
  CHAR    field_06[4];       // 交易代碼
  CHAR    field_07[4];       // 回應代碼
  CHAR    field_08[8];       // 刷卡機端末代號  右靠左補零
  CHAR    field_09[15];      // 刷卡機商店代號  右靠左補零
  CHAR    field_10[8];       // 交易日期
  CHAR    field_11[6];       // 交易時間   74 byte
  union
     {
     TCHAR    field_specil[226]; // 特別欄位
     HWEDC_Data_0x50_FETCspec fetc;
     HWEDC_Data_0x50_TSCCspec tscc;
     }u;
} HWEDC_Data_0x50;  //總長度 300




typedef struct //刷卡機交易回應訊息格式 HWEDC_Data_0x40;
{
  CHAR    field_01[1];       // 訊息代碼        固定為 CHAR(40)
  CHAR    field_02[6];       // 店號            右靠左補零
  CHAR    field_03[8];       // 收銀機代號      右靠左補零
  CHAR    field_04[8];       // 收銀機交易序號
  CHAR    field_05[6];       // 收銀員代碼      左靠右補空白
  CHAR    field_06[4];       // 交易代碼
  CHAR    field_07[4];       // 回應代碼
  CHAR    field_08[8];       // 刷卡機端末代號  右靠左補零
  CHAR    field_09[15];      // 刷卡機商店代號  右靠左補零
  CHAR    field_10[8];       // 交易日期
  CHAR    field_11[6];       // 交易時間
  CHAR    field_specil[152]; // 特別欄位
  CHAR    field_speci2[50];  // 電子發票載具   2018/01/22   Lu update
  CHAR    field_speci3[124];  // 保留欄位       2018/01/22   Lu update
} HWEDC_Data_0x40;    //總長度 226 + 50 + 124 = 400 


typedef struct //刷卡機交易回應訊息格式特別欄位展開之 1 (信用卡/AE卡/大來卡 : 消費,取消)
{
  CHAR    field_01[30];      // 卡號            左靠右補空白
  CHAR    field_02[12];      // 交易參考號碼
  CHAR    field_03[6];       // 交易調閱編號    右靠左補零
  CHAR    field_04[6];       // 刷卡機批次號    右靠左補零
  CHAR    field_05[6];       // 交易授權碼
  CHAR    field_06[12];      // 原始交易金額    整數 10 , 小數 2
  CHAR    field_07[12];      // 實際交易金額    整數 10 , 小數 2
  CHAR    field_08[9];       // 本次折抵點數    右靠左補零
  CHAR    field_09[12];      // 本次折抵金額    整數 10 , 小數 2
  CHAR    field_10[9];       // 剩餘點數        右靠左補零
  CHAR    field_11[1];       // 讀卡方式
  CHAR    field_12[1];       // 簽名
  CHAR    field_13[1];       // 卡片類別
  CHAR    field_14[10];      // 卡片類別名稱
  CHAR    field_15[25];      // 保留欄位        空白
} HWEDC_Data_0x40_spec1;  //總長度 152


typedef struct //刷卡機交易回應訊息格式特別欄位展開之 2 (油摺卡 : 消費,取消)
{
  CHAR    field_01[30];      // 卡號            左靠右補空白
  CHAR    field_02[12];      // 交易參考號碼
  CHAR    field_03[6];       // 交易調閱編號    右靠左補零
  CHAR    field_04[6];       // 刷卡機批次號    右靠左補零
  CHAR    field_05[6];       // 交易授權碼
  CHAR    field_06[12];      // 原始交易金額    整數 10 , 小數 2
  CHAR    field_07[12];      // 實際交易金額    整數 10 , 小數 2
  CHAR    field_08[1];       // 油摺卡別
  CHAR    field_09[6];       // 油品數量        整數 4 , 小數 2
  CHAR    field_10[10];      // 油品類別        左靠右補空白
  CHAR    field_11[10];      // 車號            左靠右補空白
  CHAR    field_12[6];       // 里程數          右靠左補零
  CHAR    field_13[35];      // 保留欄位        空白
} HWEDC_Data_0x40_spec2;  //總長度 152


typedef struct //刷卡機交易回應訊息格式特別欄位展開之 3 (SVC交易 : 消費,消費取消,加值,加值取消)
{
  CHAR    field_01[30];      // 卡號            左靠右補空白
  CHAR    field_02[12];      // 交易參考號碼
  CHAR    field_03[6];       // 交易調閱編號    右靠左補零
  CHAR    field_04[6];       // 刷卡機批次號    右靠左補零
  CHAR    field_05[6];       // 交易授權碼
  CHAR    field_06[12];      // 原始交易金額    整數 10 , 小數 2
  CHAR    field_07[12];      // 實際交易金額    整數 10 , 小數 2
  CHAR    field_08[8];       // 卡片交易序號    右靠左補零
  CHAR    field_09[8];       // 卡片扣款序號    右靠左補零
  CHAR    field_10[8];       // 卡片加值序號    右靠左補零
  CHAR    field_11[1];       // AutoLoad        "Y" 或 "N"
  CHAR    field_12[12];      // AutoLoad 金額   整數 10 , 小數 2
  CHAR    field_13[8];       // 現金點餘額      右靠左補零
  CHAR    field_14[8];       // 紅利點餘額      右靠左補零
  CHAR    field_15[1];       // 卡片類別        "1"=銀行卡 "2"=遠通卡
  CHAR    field_16[14];      // 保留欄位        空白
} WEDC_Data_0x40_spec3;  //總長度 152


typedef struct //刷卡機交易回應訊息格式特別欄位展開之 4 (信用卡/AE卡/大來卡/油摺卡 : 結帳,總帳查詢)
{
  CHAR    field_01[6];       // 批次號          右靠左補零
  CHAR    field_02[6];       // 銷售總筆數      右靠左補零
  CHAR    field_03[12];      // 銷售總金額      右靠左補零
  CHAR    field_04[6];       // 退貨總筆數      右靠左補零
  CHAR    field_05[12];      // 退貨總金額      右靠左補零
  CHAR    field_06[110];     // 保留欄位        空白
} HWEDC_Data_0x40_spec4;  //總長度 152


typedef struct //刷卡機交易回應訊息格式特別欄位展開之 5 (SVC交易 : 結帳,總帳查詢)
{
  CHAR    field_01[6];       // 批次號          右靠左補零
  CHAR    field_02[6];       // 銷售總筆數      右靠左補零
  CHAR    field_03[12];      // 銷售總金額      右靠左補零
  CHAR    field_04[6];       // 退貨總筆數      右靠左補零
  CHAR    field_05[12];      // 退貨總金額      右靠左補零
  CHAR    field_06[6];       // 信用加值總筆數  右靠左補零
  CHAR    field_07[12];      // 信用加值總金額  右靠左補零
  CHAR    field_08[6];       // 現金加值總筆數  右靠左補零
  CHAR    field_09[12];      // 現金加值總金額  右靠左補零
  CHAR    field_10[74];      // 保留欄位        空白
} HWEDC_Data_0x40_spec5;  //總長度 152


typedef struct //刷卡機交易回應訊息格式特別欄位展開之 6 (SVC交易 : 餘額查詢)
{
  CHAR    field_01[30];      // 卡號            左靠右補空白
  CHAR    field_02[8];       // 現金點餘額      右靠左補零
  CHAR    field_03[8];       // 紅利點餘額      右靠左補零
  CHAR    field_04[1];       // 卡片類別
  CHAR    field_05[105];     // 保留欄位        空白
} HWEDC_Data_0x40_spec6;  //總長度 152


typedef struct //刷卡機交易回應訊息格式特別欄位展開之 7 (異常報表/總帳/明細 列印)
{
  CHAR    field_01[152];     // 保留欄位        空白
} HWEDC_Data_0x40_spec7;  //總長度 152


typedef struct //刷卡機交易回應訊息格式特別欄位展開之 8 (異常報表/總帳/明細 列印)
{
  CHAR    field_01[30];      // 卡號            左靠右補空白
  CHAR    field_02[122];     // 保留欄位        空白
} HWEDC_Data_0x40_spec8;  //總長度 152


typedef struct //刷卡機交易回應訊息格式特別欄位展開之 9 (信用卡+SVC 結帳)
{
  CHAR    field_01[6];       // 批次號          右靠左補零 (信用卡)
  CHAR    field_02[6];       // 銷售總筆數      右靠左補零 (信用卡)
  CHAR    field_03[12];      // 銷售總金額      右靠左補零 (信用卡)
  CHAR    field_04[6];       // 退貨總筆數      右靠左補零 (信用卡)
  CHAR    field_05[12];      // 退貨總金額      右靠左補零 (信用卡)
  CHAR    field_06[8];       // 刷卡機端末代號  右靠左補零 (SVC)
  CHAR    field_07[15];      // 刷卡機商店代號  右靠左補零 (SVC)
  CHAR    field_08[6];       // 刷卡機批次號    右靠左補零 (SVC)
  CHAR    field_09[6];       // 銷售總筆數      右靠左補零 (SVC)
  CHAR    field_10[12];      // 銷售總金額      右靠左補零 (SVC)
  CHAR    field_11[6];       // 退貨總筆數      右靠左補零 (SVC)
  CHAR    field_12[12];      // 退貨總金額      右靠左補零 (SVC)
  CHAR    field_13[6];       // 信用加值總筆數  右靠左補零 (SVC)
  CHAR    field_14[12];      // 信用加值總金額  右靠左補零 (SVC)
  CHAR    field_15[6];       // 現金加值總筆數  右靠左補零 (SVC)
  CHAR    field_16[12];      // 現金加值總金額  右靠左補零 (SVC)
  CHAR    field_17[9];       // 保留欄位        空白
} HWEDC_Data_0x40_spec9;  //總長度 152


/////////////////////////////////////    2016/01/12 Update     ///////////////////////////////////////////

typedef struct //銀聯卡交易 : 消費,退貨   2010,2030
{
  CHAR    field_01[30];      // 卡號            左靠右補空白
  CHAR    field_02[12];      // 交易參考號碼
  CHAR    field_03[6];       // 交易調閱編號    右靠左補零
  CHAR    field_04[6];       // 刷卡機批次號    右靠左補零
  CHAR    field_05[6];       // 交易授權碼
  CHAR    field_06[12];      // 原始交易金額    整數 10 , 小數 2
  CHAR    field_07[12];      // 實際交易金額    整數 10 , 小數 2
  CHAR    field_08[9];       // 本次折抵點數    右靠左補零
  CHAR    field_09[12];      // 本次折抵金額    右靠左補零
  CHAR    field_10[9];       // 剩餘點數        右靠左補零
  CHAR    field_11[1];       // 交易讀卡方式    "S,I,W"
  CHAR    field_12[1];       // 簽單簽名
  CHAR    field_13[6];       // 銀聯卡交易序號  右靠左補零
  CHAR    field_14[4];       // 銀聯卡交易日期  右靠左補零
  CHAR    field_15[4];       // 銀聯卡交易日期
  CHAR    field_16[6];       // 銀聯卡交易時間
  CHAR    field_17[16];      // 保留欄位        空白

} HWEDC_Data_0x40_spec10;  //總長度 152



typedef struct //晶片金融卡 : 消費,退貨  3010,3030
{
  CHAR    field_01[30];      // 卡號            左靠右補空白
  CHAR    field_02[12];      // 交易參考號碼
  CHAR    field_03[6];       // 交易調閱編號    右靠左補零
  CHAR    field_04[6];       // 刷卡機批次號    右靠左補零
  CHAR    field_05[6];       // 交易授權碼
  CHAR    field_06[12];      // 原始交易金額    整數 10 , 小數 2
  CHAR    field_07[12];      // 實際交易金額    整數 10 , 小數 2
  CHAR    field_08[8];       // 本次折抵點數    右靠左補零
  CHAR    field_09[1];       // 本次折抵金額    右靠左補零
  CHAR    field_10[1];       // 剩餘點數        右靠左補零
  CHAR    field_11[66];       // 保留欄位        空白

} HWEDC_Data_0x40_spec11;  //總長度 152


typedef struct
{
  CHAR cTotalCount[2];
  CHAR cSendCount[2];
  CHAR cReceiveCount[2];

}  DownLoadInfo;


typedef struct
{
  CHAR  uMsgid[1];
  CHAR  cStatus[2];
  CHAR  cLen[2];
  CHAR  cErmType[1];
  CHAR  cFileSize[2];
  CHAR  cCompressFlag[1];
  DownLoadInfo sDownLoadInfo;
  CHAR cBlockLen[2];
  CHAR uBlockData[4096];

}  HWEDC_Data_0x42;


/****
typedef struct
{
  unsigned short int  iTotalCount;
  unsigned short int  iSendCount;
  unsigned short int  iReceiveCount;

}  DownLoadInfo;


typedef struct
{
  uchar uMsgid[1];
  unsigned short int  iStatus;
  unsigned short int  iLen;
  CHAR cErmType[1];
  unsigned short int  iFileSize;
  CHAR cCompressFlag[1];
  DownLoadInfo sDownLoadInfo;
  unsigned short int  iBlockLen;
  uchar uBlockData[4096];

}  HWEDC_Data_0x42;

*****/




/*
 *-----------------------------------------------------------------------------
 * End of Include file
 *-----------------------------------------------------------------------------
 */
