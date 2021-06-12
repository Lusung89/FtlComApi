// FTLComTest.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "FTLComTest.h"
//#include "..\\FtlComApi.h" 

//https://www.youtube.com/watch?v=auNPlHYB4ss
#include "FtlComApi.h"   // Ref : FTLComTest properity page -> C/C++ ->general-> additional include directory :
                         // Add Lib File                      Linker->Input -> additional Dependence 
                         //                                   Linker->general-> additional Liberary directory  ...$(intDir)


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <atlstr.h> 
#include <tchar.h>
#include <string>
#include <sstream> 

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance

WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
CHAR gCCardNo[31];
HWND hwd_IDD_FORMVIEW;
HWND hwd_IDD_FORMPPAGE;
wstring wsEdcMsg;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    FormviewEdcTet(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    FormPROPPAGEPrc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    FormPROPPAGEPrc(HWND, UINT, WPARAM, LPARAM);


// for FtlComApi.dll test
int  FtlComApiDll_Test(int TranType, int iPayAmt);

inline UINT AddString(const HWND hList, const wstring& s);
inline UINT CleaList(const HWND hList);
inline UINT Static33Text(const wstring& s);
inline UINT Edit1Text(const wstring& s);
inline UINT GetEdit1Text(wstring& s);

inline UINT InitCBComPortText();


int EDCPayAmt;



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FTLCOMTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FTLCOMTEST));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FTLCOMTEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FTLCOMTEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   EDCPayAmt = 3100;
   hwd_IDD_FORMVIEW=NULL;
   hwd_IDD_FORMPPAGE=NULL;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//              his message is sent whenever a user interaction occurs, such as menu item selection,
//              or when a parent receives a notification from a child window.
//              Ref: https://www.codementor.io/@malortie/build-win32-api-app-windows-messages-c-cpp-visual-studio-du107sbya
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//  WM_SIZE
//  WM_CREATE
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    /**/
    case WM_CREATE:
    {
        // Create Dialog windows
        hwd_IDD_FORMVIEW = CreateDialog(hInst, MAKEINTRESOURCE(IDD_FORMVIEW), hWnd, (DLGPROC)FormviewEdcTet);
        // Show  Dialog window 
        //ShowWindow(hwd_IDD_FORMVIEW, SW_SHOWNA);
        hwd_IDD_FORMPPAGE = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PROPPAGE_SMALL), hWnd, (DLGPROC)FormPROPPAGEPrc);
        //ShowWindow(hwd_IDD_FORMPPAGE, SW_SHOWNA);

        break;
    }
    /**/
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
               DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
               // DialogBox(hInst, MAKEINTRESOURCE(IDD_FORMVIEW), hWnd, FormviewEdcTet);
               // DialogBox(hInst, MAKEINTRESOURCE(IDD_FORMVIEW), hWnd, About);

                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            case ID_FTLCOMSOURCE_TEDCAPIINI:
                //UTEDCAPIINI();
                if(hwd_IDD_FORMPPAGE == NULL)
                    hwd_IDD_FORMPPAGE = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PROPPAGE_SMALL), hWnd, (DLGPROC)FormPROPPAGEPrc);
                ShowWindow(hwd_IDD_FORMPPAGE, SW_SHOWNORMAL); // SW_SHOWNA);
                //MessageBox(L"File > New menu option");
                break;

            case ID_FTLCOMSOURCE_TEDCAPICOMTEST:
             {
                 UTEDCAPICOMTEST(); 
                
                break;
            }

            case ID_TEDCAPICALL_T703:
            {
                MessageBox(NULL, L"UTEDCAPICALL_T703() UTEDCAPIINI() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                UTEDCAPIINI();
                MessageBox(NULL, L"UTEDCAPICALL_T703() UTEDCAPIINI() END ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                break;
            }
            case ID_TEDCAPICALL_T704:
            {
                MessageBox(NULL, L"UTEDCAPICALL_T704() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                UTEDCAPICALL_T704(EDCPayAmt);
                MessageBox(NULL, L"UTEDCAPICALL_T704() END ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                break;
            }
            case ID_TEDCAPICALL_T705:
            {
                MessageBox(NULL, L"UTEDCAPICALL_T705() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                UTEDCAPIINI();
                MessageBox(NULL, L"UTEDCAPICALL_T705() END ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                break;
            }
            case ID_TEDCAPICALL_T707:
            {
                MessageBox(NULL, L"UTEDCAPICALL_T707() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                UTEDCAPICALL_T707(EDCPayAmt);
                MessageBox(NULL, L"UTEDCAPICALL_T707() END ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                break;
            }
            case ID_TEDCAPICALL_T708:
            {
                MessageBox(NULL, L"UTEDCAPICALL_T708() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                UTEDCAPICALL_T708(EDCPayAmt);
                MessageBox(NULL, L"UTEDCAPICALL_T708() END ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                break;
            }
            case ID_TEDCAPICALL_T709:
            {
                MessageBox(NULL, L"UTEDCAPICALL_T709() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                UTEDCAPICALL_T709(EDCPayAmt);
                MessageBox(NULL, L"UTEDCAPICALL_T709() END ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                break;
            }
               
            case ID_TEDCAPICALLSIG_T710:
            {
                MessageBox(NULL, L"UTEDCAPICALL_T710() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                UTEDCAPICALL_SIG_T710(EDCPayAmt);
                MessageBox(NULL, L"UTEDCAPICALL_T710() END ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                break;
            }

            case ID_TEDCAPICALLSIG_T711:
            {
                MessageBox(NULL, L"UTEDCAPICALL_T711() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                UTEDCAPICALL_SIG_T711(EDCPayAmt);
                MessageBox(NULL, L"UTEDCAPICALL_T711() END ....", L"Msg title", MB_OK | MB_ICONQUESTION);
                break;
            }

            case ID_TEDCAPICALLSIG_T712:
            {// Create Dialog windows
                if( hwd_IDD_FORMVIEW==NULL)
                    hwd_IDD_FORMVIEW = CreateDialog(hInst, MAKEINTRESOURCE(IDD_FORMVIEW), hWnd, (DLGPROC)FormviewEdcTet);
                // Show  Dialog window 
                ShowWindow(hwd_IDD_FORMVIEW, SW_SHOWNA);

                break;
            }
            case ID_FTLCOMAPI_DLLTEST:
            {
                
                int ii = FtlComApiDll_Test(0, 3100);
                wstring ss;
                ss= L"ID_FTLCOMAPI_DLLTEST END .... rtn = " + _StringToWString( _int2str(ii) ); 
                MessageBox(NULL, ss.c_str() , L"Msg title", MB_OK | MB_ICONQUESTION);
                break;
            }


            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



// Message handler for FormviewEdcTet box.
INT_PTR CALLBACK FormviewEdcTet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    UNREFERENCED_PARAMETER(lParam);
    //DWORD           dwResult;
    //TCHAR           szLabelType[256];
    //TCHAR           szLen[MAX_PATH];
    //TCHAR           szMsgBuf[256];
    //LPSCAN_BUFFER   lpScanBuf;
    //HWND hwd_static1, hwd_static2, hwd_List1;

   //hwd_static1 = GetDlgItem(hDlg, IDC_STATIC1);  
   // hwd_static2 = GetDlgItem(hDlg, IDC_STATIC2);
   // hwd_static3 = GetDlgItem(hDlg, IDC_STATIC3);
   // SetDlgItemInt(hwnd, IDC_SHOWCOUNT, data, FALSE);
   // SetDlgItemText(hwd_static2, IDC_STATIC2, L"This is a Test string");
   // ShowWindow(hwd_static2, SW_SHOW);
    switch (message)
    {
    case WM_CREATE:
    {
        
        break;
    }
    case WM_INITDIALOG:
    {
       
        return (INT_PTR)TRUE;
    }
    case WM_SHOWWINDOW:
    {
        wstring wsAmt;
        wsAmt = _StringToWString(_int2str(EDCPayAmt));
        Edit1Text(wsAmt);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        
        if (LOWORD(wParam) == IDC_BUTTON1)  //DC查詢707
        {
           // HWND  hwd_List1, hwd_edit1;

            //hwd_List1 = GetDlgItem(hDlg, IDC_EDIT1);
            //SetDlgItemText(hDlg, IDC_EDIT1, _T("This is a string"));
            //SetDlgItemText(hDlg, IDC_STATIC1, _T("C++ Win32 Application"));

            //hwd_List1 = GetDlgItem(hDlg, IDC_LIST1);
            wstring ws;
            GetEdit1Text(ws);
            AddString(NULL, ws);
            //AddString(NULL, _T("Listbox"));
            //EndDialog(hDlg, LOWORD(wParam));
 
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON2)   //EDC支付704
        {
            // CleaList(NULL);
            //EndDialog(hDlg, LOWORD(wParam));
            //return (INT_PTR)TRUE;

            //wstring wsAmt;
           //wsAmt = _StringToWString(_int2str(EDCPayAmt));
           // Edit1Text(wsAmt);
           ;;
        }
        else if (LOWORD(wParam) == IDC_BUTTON3)  //EDC電簽710
        {
           CleaList(NULL);
           wstring wsAmt,slog;
           int iamt;
           GetEdit1Text(wsAmt);
           iamt = _StrToInt( _WStringToString(wsAmt) );
           //wsAmt = _StringToWString(_int2str(EDCPayAmt));
           // Edit1Text(wsAmt);

           slog = _T("★★ Start UTEDCAPICALL_DBtn(): $") + wsAmt + _T(" ★★");
           AddString(NULL, slog);
           UTEDCAPICALL_DBtn3(iamt);
           slog = _T("★★ END UTEDCAPICALL_DBtn(): $") + wsAmt + _T(" ★★");
           AddString(NULL, slog);

           return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON8)
        {
            CleaList(NULL);
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON10)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else
        {
            ;;
        }


       // break;
    }
    return (INT_PTR)FALSE;
}


// Message handler for Form_IDD_PROPPAGE box.
//https://docs.microsoft.com/en-us/windows/win32/controls/create-a-simple-combo-box
//https://wenku.baidu.com/view/49c892cbe53a580217fcfe25.html
INT_PTR CALLBACK FormPROPPAGEPrc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    
    switch (message)
    {
    case WM_CREATE:
    {

        break;
    }
    case WM_INITDIALOG:
    {
        
        break;
       // return (INT_PTR)TRUE;
    }
    case WM_SHOWWINDOW:
    {
        InitCBComPortText();
        break;
        //return (INT_PTR)TRUE;
    }
    case WM_COMMAND:

        if (LOWORD(wParam) == IDC_BUTTON1)  
        {

            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON2)   // EXIT
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
            
        }
        else if (LOWORD(wParam) == IDC_BUTTON3) 
        {
           
            return (INT_PTR)TRUE;
        }
        else
        {
            //EndDialog(hDlg, LOWORD(wParam));
            ;;
        }


        // break;
    }
    return (INT_PTR)FALSE;

}



//HWND hwd_IDD_FORMVIEW 
//HWND  hwd_List1; // of Dialog windows
//hwd_List1 = GetDlgItem(hDlg, IDC_LIST1);
//AddString(hwd_List1, _T("Listbox"));
inline UINT AddString(const HWND hList, const wstring& s)
{
    HWND hwd_List1;

    if (hList == NULL)
        hwd_List1 = GetDlgItem(hwd_IDD_FORMVIEW, IDC_LIST1);
    else
        hwd_List1 = hList;


    if (s.length() > 0)
    {
        return static_cast<UINT>(SendMessage(hwd_List1, LB_ADDSTRING, 0,
            reinterpret_cast<LPARAM>(s.c_str())));
    }
    return 0;
}

// Clear List
// SendMessage(DlgIndex, LB_RESETCONTENT, 0, 0);
inline UINT CleaList(const HWND hList)
{
    HWND hwd_List1;

    if (hList == NULL)
        hwd_List1 = GetDlgItem(hwd_IDD_FORMVIEW, IDC_LIST1);
    else
        hwd_List1 = hList;

    return static_cast<UINT>(SendMessage(hwd_List1, LB_RESETCONTENT, 0, 0) );
    //return static_cast<UINT>(SendMessage(hList, LB_RESETCONTENT, 0, 0));
}


inline UINT Static33Text(const wstring& s)
{
    return SetDlgItemText(hwd_IDD_FORMVIEW, IDC_STATIC3, s.c_str());
    //SetDlgItemText(hwd_IDD_FORMVIEW, IDC_STATIC3, _T("C++ Win32 Application"));
}


inline UINT Edit1Text(const wstring& s)
{
   return SetDlgItemText(hwd_IDD_FORMVIEW, IDC_EDIT1, s.c_str());
    //SetDlgItemText(hwd_IDD_FORMVIEW, IDC_EDIT1, _T("This is a string"));
}



//GetDlgItemTextLength()
//int nTimes = GetDlgItemInt(hwnd, IDC_NUMBER, &bSuccess, FALSE);
inline UINT GetEdit1Text( wstring& s)
{
    int len = GetWindowTextLength(GetDlgItem(hwd_IDD_FORMVIEW, IDC_EDIT1));
    if (len > 0)
    {
        int i;
        //TCHAR* buf;
        //buf = (TCHAR*)GlobalAlloc(GPTR, len + 1);
        
        TCHAR buf[1024];
        GetDlgItemText(hwd_IDD_FORMVIEW, IDC_EDIT1, buf, len + 1);
        s = wstring(buf);

        //... do stuff with text ...
        //GlobalFree((HANDLE)buf);  //fo//buf = (TCHAR*)GlobalAlloc(GPTR, len + 1);
    }

    return len;
}




///////////////////////////////////////////////////////////////////////////////////////////////////

int  FtlComApiDll_Test( int TranType, int iPayAmt)
{
    int result;
    //i = fnFtlComApi();  // FtlComApi.dll -> fnFtlComApi()
    //i = FtlComApiInit();
    //i = FtlComApiTest(0, 0, NULL, NULL, 0, NULL);

    int  iTMode, iAmt, rtn, iTranNo;
    string slog, sDttm, sBandCode, sCard_no;
    char InData[1024], OutData[1024], cmd[5];
    char RepCode[1024], RepData[1024];


    rtn = 0; TranType = 707;  iTMode = 0;
    iAmt = iPayAmt;
    sprintf_s(cmd, "FE04");

    sDttm = _currentDateTime(1, 0);
    iTranNo = _StrToInt(sDttm.substr(8, 6));
    sprintf_s(InData, "%6s%-8s%08d%6s%10s%-14s%06d%4s%04d", "009939", "00000001", iTranNo, "999999", "XX01234567", sDttm.c_str(), iAmt, cmd, 0);
   
   // result = FtlComApi(TranType, iTMode, (byte*) InData, (byte*)OutData, iAmt, (byte*)cmd);  // FtlComApi.dll
    _MySleep(10);
  //  if (result != SUCCEED)
  //      goto ENDFtlComApiDll;


    HWEDC_Data_0x31 strx31;
    memcpy(&strx31, OutData, sizeof(strx31));
    memcpy(gCCardNo, &strx31.field_14, 30);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    rtn = 0; TranType = 704;  iTMode = 0;
    iAmt = iPayAmt;  //3100
    sprintf_s(cmd, "8010");
    sBandCode = "0030";       //(66,4);     2017/07/31
    sCard_no = string(gCCardNo); //     "438045159"               //"431195100XXXXXXX";       //2018/05/31
    sprintf_s(InData, "%6s%-8s%08d%-6s%-10s%-14s%06d%4s%4s%4s%-30.30s",
        "009939", "01", iTranNo, "999999", "XX01234567", sDttm.c_str(), iAmt, cmd, sBandCode.c_str(), "0812", sCard_no.c_str());
   
   // result = FtlComApi(TranType, iTMode, (byte*)InData, (byte*)OutData, iAmt, (byte*)cmd); // FtlComApi.dll
   // if (result != SUCCEED)
   //     goto ENDFtlComApiDll;
     
    _MySleep(100);
    // RepCode[1024], RepData[1024];
   // FtlComApiGetRep(TranType, iTMode, (byte*)RepCode , (byte*)RepData, rtn);  // FtlComApi.dll
 ////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (iAmt > 3000)
    {

        TranType = 710;
        sprintf_s(InData, "S");
        sprintf_s(cmd, "0000");
       // result = FtlComApi(TranType, iTMode, (byte*)InData, (byte*)OutData, iAmt, (byte*)cmd);  // FtlComApi.dll
       // if (result != SUCCEED)
       //     goto ENDFtlComApiDll;

        _MySleep(10);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        rtn = 0; TranType = 711;  iTMode = 0;

        //sDttm = _currentDateTime(1, 0);
        //iTranNo = _StrToInt(sDttm.substr(8, 6));

        sprintf_s(InData, "S");
        sprintf_s(cmd, "0000");

       // result = FtlComApi(TranType, iTMode, (byte*)InData, (byte*)OutData, iAmt, (byte*)cmd);  // FtlComApi.dll
        _MySleep(10);

    }
    

ENDFtlComApiDll:
    result = SUCCEED;

    if (result == SUCCEED)
    {
        _Strprintf(slog, "FtlComApiDll_Test() ==> result=%d , RepData=%s", result, RepData);
    }
    else
    {
        _Strprintf(slog, "FtlComApiDll_Test() ==> result=%d ErrorCode:%s, ErrorMsg:xxxx...", result, RepCode);
    }

   
    wsEdcMsg = _StringToWString(slog); // ANSII String to Unicode


    /*****
    switch (TranType)
    {
        //int __fastcall cLsEDC::EdcApiCall(int TranType, int TMode, byte * lpbInData, byte * lpbOutData, int iAmt, byte * Cmd)
    case 0:
        result = FtlComApiTest(0, 0, NULL, NULL, 0, NULL);
        //sprintf_s(cEDCHostResponseCode, "0000");
        //sprintf_s(cEDCTscData, "0000");
        //sprintf_s(cEDCErrMsg, "REQUEST_TEST_OK");
        //iEDCTscDataLen = 4; //0000
        break;
  
    case 704:  //信用卡交易 0010:信用卡, 0011:紅利 ,8010:信用卡, 2010:銀聯, 3010:SmartPay
    case 705:  //信用卡退貨 0030, 0031
    case 707:   // "FE01, FE02 FE03, FE04  SmartPay卡片查詢"     ; FE04 Lusung Add 2018/06/01
    case 708:  // FETC 存在
    case 709:  // FETC 結帳
    case 710:  //電簽啟動訊息
    case 711:  //電簽確認訊息
    {               
        //FtlComApi(int TranType, int TMode, byte * InData, byte * OutData, int iAmt, byte * Cmd)
        result = FtlComApi(TranType, 0, InData, OutData, iAmt, Cmd);

        //EDCErrorCode = result;
        //iEDCTscDataLen = CommEDC->iTscDataLen;
       // memcpy(cEDCHostResponseCode, CommEDC->ucHostResponseCode, sizeof(CommEDC->ucHostResponseCode));
       // memcpy(cEDCTscData, CommEDC->ucTscData, CommEDC->iTscDataLen);
       // memcpy(cEDCErrMsg, CommEDC->sTsccApiErrorMsg.c_str(), CommEDC->sTsccApiErrorMsg.length());
        break;
    }

    default:
        result = REQUEST_NOT_OK;
        //sprintf_s(cEDCHostResponseCode, "60A1");
        //sprintf_s(cEDCTscData, "60A1");
        //sprintf_s(cEDCErrMsg, "REQUEST_NOT_OK");
        //iEDCTscDataLen = 4; //60A1
    }
   ****/

    return (result);
}








 //ShowMessage(_T"UTEDCAPICOMTEST OK ");

int UTEDCAPIINI() 
{
    
    int rtn = 0;
    string slog;
    
    memset(gCCardNo, 0, sizeof(gCCardNo));

    cLsEDC* CommEDC = new cLsEDC();
    rtn = CommEDC->EDCApiInit(0, 0, "0123456789");
 
    if (rtn == SUCCEED)
    {
        _Strprintf(slog, "UTEDCAPIINI() ==> rtn=%d ...", rtn);
    }
    else
    {
        _Strprintf(slog, "UTEDCAPIINI()  ==> rtn=%d ErrorCode:%s, ErrorMsg:%s...", rtn, CommEDC->ucHostResponseCode, CommEDC->sTsccApiErrorMsg.c_str());
    }

    delete CommEDC;
    wsEdcMsg = _StringToWString(slog); // ANSII String to Unicode

   //Display a Variable in MessageBox c++
   // MessageBox(NULL, _StringToWString(slog).c_str(), L"Msg title", MB_OK | MB_ICONQUESTION);
    //MessageBox(NULL, L"UTEDCAPIINI() ==>new cLsEDC() OK ", L"Msg title", MB_OK | MB_ICONQUESTION);

   return(0);
}



int UTEDCAPICOMTEST()
{

    int rtn = 0;
    string slog;

   // MessageBox(NULL, L"UTEDCAPICOMTEST() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
    cLsEDC* CommEDC = new cLsEDC();
    for (int i = 1; i <= 10; i++)
    {
        _Strprintf(slog, "UTEDCAPICOMTEST() ==> Test(%03d) ...", i);
        rtn = CommEDC->EDCApiComTest(0, 0, "0123456789");
        if (rtn)
            break;
        _MySleep(500);
        //Sleep(1000);
    }
    if (rtn == SUCCEED)
    {
        _Strprintf(slog, "UTEDCAPICOMTEST() ==> rtn=%d ...", rtn);
    }
    else
    {
        _Strprintf(slog, "UTEDCAPICOMTEST()  ==> rtn=%d ErrorCode:%s, ErrorMsg:%s...", rtn, CommEDC->ucHostResponseCode, CommEDC->sTsccApiErrorMsg.c_str());
    }

    delete CommEDC;
    wsEdcMsg = _StringToWString(slog); // ANSII String to Unicode
  
    return(0);
      
}

void _MySleep(int islp)
{
    for (int i = 0; i < islp;  i++)
    {
        _DoEvents();
        Sleep(1);
     }
}

void _MyDoEvents()
{
    MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
    {
        if (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
            break;
    }
}


// 不再使用
int UTEDCAPICALL_T703(int PayAmt)
{
    //EdcApiCall(int TranType, int TMode, byte * lpbInData, byte * lpbOutData, int iAmt, byte * Cmd)

    int iTranType, iTMode, iAmt, rtn, iTranNo;
    string slog, sDttm;
    char InData[1024], RepData[1024], cmd[5];
    
   // MessageBox(NULL, L"UTEDCAPICALL_T703() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
   
    rtn = 0; iTranType = 703;  iTMode = 0;
    iAmt = PayAmt;
    sprintf_s(cmd, "FE01");

    /*
    sTenCode = s.substr(1, 6);
    sEcrNo = s.substr(7, 8);
    sTranNo = s.substr(15, 8);
    sSalesNo = s.substr(23, 6);
    sInvoNo = s.substr(29, 10);;
    //14
    //6        
    sTranCode = s.substr(59, 4);  //(58,4);     2013/05/22
    sTimeOut = s.substr(62, 4);    //(62,4);
    */

    sDttm = _currentDateTime(1, 0);
    iTranNo = _StrToInt(sDttm.substr(8, 6));

    sprintf_s(InData,"%6s%-8s%08d%6s%10s%-14s%-6s%4s%04d", "009939","01", iTranNo, "999999","XX01234567", "0","0", cmd, 4);
    
    cLsEDC* CommEDC = new cLsEDC();
   
    rtn = CommEDC->EdcApiCall(iTranType, iTMode, (byte*)InData, (byte*)RepData, iAmt, (byte*)cmd);
    
     _MySleep(10);
     if (rtn == SUCCEED)
     {
         _Strprintf(slog, "UTEDCAPICALL_T703() ==> rtn=%d ...", rtn);
     }
     else
     {
         _Strprintf(slog, "UTEDCAPICALL_T703() ==> rtn=%d ErrorCode:%s, ErrorMsg:%s...", rtn, CommEDC->ucHostResponseCode, CommEDC->sTsccApiErrorMsg.c_str());
     }

     delete CommEDC;
     wsEdcMsg = _StringToWString(slog); // ANSII String to Unicode
    
    //_Strprintf(slog, "UTEDCAPICALL_T703() ==> rtn=%d ...", rtn);
    //MessageBox(NULL, _StringToWString(slog).c_str(), L"Msg title", MB_OK | MB_ICONQUESTION);

    return(rtn);
     
}



// 信用卡消費 : 交易前要先執行 T707 取得卡號,,包含電簽啟動 
int UTEDCAPICALL_T704(int PayAmt)
{
    int iTranType, iTMode, iAmt, rtn, iTranNo;
    string slog, sDttm, sBandCode, sCard_no;
    char InData[1024], RepData[1024], cmd[5];

        
    rtn = 0; iTranType = 704;  iTMode = 0;
    iAmt = PayAmt;  //3100
    sDttm = _currentDateTime(1, 0);
    iTranNo = _StrToInt(sDttm.substr(8, 6));
    sprintf_s(cmd, "8010");
    sBandCode = "0030";       //(66,4);     2017/07/31
    sCard_no = string(gCCardNo); //     "438045159"               //"431195100XXXXXXX";       //2018/05/31
    sprintf_s(InData, "%6s%-8s%08d%-6s%-10s%-14s%06d%4s%4s%4s%-30.30s",
                  "009939", "01", iTranNo, "999999", "XX01234567", sDttm.c_str(), iAmt, cmd, sBandCode.c_str(), "0812", sCard_no.c_str());
   
   
    cLsEDC* CommEDC = new cLsEDC();
    rtn = CommEDC->EdcApiCall(iTranType, iTMode, (byte*)InData, (byte*)RepData, iAmt, (byte*)cmd);
    //_MySleep(100);  不可以有delay
   
    if (iAmt > 3000)
    {
        
        iTranType = 710;
        sprintf_s(InData, "S");
        sprintf_s(cmd, "0000");
        //cLsEDC* CommEDC = new cLsEDC();
        rtn = CommEDC->EdcApiCall(iTranType, iTMode, (byte*)InData, (byte*)RepData, iAmt, (byte*)cmd);
        //delete CommEDC;
        _MySleep(10);
    }

    if (rtn == SUCCEED)
    {
        _Strprintf(slog, "UTEDCAPICALL_T704() ==> rtn=%d ...", rtn);
    }
    else
    {
        _Strprintf(slog, "UTEDCAPICALL_T704() ==> rtn=%d ErrorCode:%s, ErrorMsg:%s...", rtn, CommEDC->ucHostResponseCode, CommEDC->sTsccApiErrorMsg.c_str());
    }

    delete CommEDC;
    wsEdcMsg = _StringToWString(slog); // ANSII String to Unicode
    
   // _Strprintf(slog, "UTEDCAPICALL_T704() END==> rtn=%d , Card_No:%30.30s...", rtn, gCCardNo);
   //MessageBox(NULL, L"UTEDCAPICALL_T704() END ....", L"Msg title", MB_OK | MB_ICONQUESTION);
    return(rtn);
}

//信用卡退貨 0030, 0031
int UTEDCAPICALL_T705(int PayAmt)
{
    int iTranType, iTMode, iAmt, rtn, iTranNo;
    string slog, sDttm, sBandCode, sChrInvoNo, s0X40Data;
    char InData[1024], RepData[1024], cmd[5];

    //MessageBox(NULL, L"UTEDCAPICALL_T705() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);

    rtn = 0; iTranType = 705;  iTMode = 0;
    iAmt = PayAmt;
    sDttm = _currentDateTime(1, 0);
    iTranNo = _StrToInt(sDttm.substr(8, 6));
    sprintf_s(cmd, "FE02");
    sBandCode = "0001";
    sChrInvoNo = "XX01234567"; // s.substr(71, 10);     //(67,4)
    s0X40Data = "00000000000000000000000000";  //s.substr(81, 226);  // 0x40 Data  226 Byte  (77,226)

    sprintf_s(InData, "%6s%-8s%08d%6s%10s%-14s%-6s%4s%04d%4s%-10s%s",
        "009939", "01", iTranNo, "999999", "XX01234567", sDttm.c_str(), "0", cmd, 4, sBandCode.c_str(), sChrInvoNo.c_str(), s0X40Data.c_str() );

    cLsEDC* CommEDC = new cLsEDC();

    rtn = CommEDC->EdcApiCall(iTranType, iTMode, (byte*)InData, (byte*)RepData, iAmt, (byte*)cmd);

    _MySleep(10);
    if (rtn == SUCCEED)
    {
        _Strprintf(slog, "UTEDCAPICALL_T705() ==> rtn=%d ...", rtn);
    }
    else
    {
        _Strprintf(slog, "UTEDCAPICALL_T705() ==> rtn=%d ErrorCode:%s, ErrorMsg:%s...", rtn, CommEDC->ucHostResponseCode, CommEDC->sTsccApiErrorMsg.c_str());
    }

    delete CommEDC;
    wsEdcMsg = _StringToWString(slog); // ANSII String to Unicode

    return(rtn);
}

// "FE01, FE02 FE03, FE04  SmartPay卡片查詢"     ; FE04 Lusung Add 2018/06/01
int UTEDCAPICALL_T707(int PayAmt)
{
    int iTranType, iTMode, iAmt, rtn, iTranNo;
    string slog, sDttm;
    char InData[1024], RepData[1024], cmd[5];

    //MessageBox(NULL, L"UTEDCAPICALL_T707() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
    //memset(gCCardNo, 0, sizeof(gCCardNo));



    rtn = 0; iTranType = 707;  iTMode = 0;
    iAmt = PayAmt;
    sprintf_s(cmd, "FE04");

    sDttm = _currentDateTime(1, 0);
    iTranNo = _StrToInt(sDttm.substr(8,6) );
    //009939 00000001 22400101 009999EF6849900020210422144434000055FE040000 NG
    // 0099390000000100140814999999XX0123456720210422140814003100FE040000 OK
    //    sprintf_s(InData, "%6s%-8s%08d%6s%10s%-14s%06d%4s%04d", "009939", "00000001", iTranNo, "999999", "XX01234567", sDttm.c_str(), iAmt, cmd, 0);
    sprintf_s(InData, "%s", "0099390000000122400101009999EF6849900020210422144434000055FE040000");
   //_Strprintf(slog, "UTEDCAPICALL_T707() ==> InData(%s)", InData);

    cLsEDC* CommEDC = new cLsEDC();

    rtn = CommEDC->EdcApiCall(iTranType, iTMode, (byte*)InData, (byte*)RepData, iAmt, (byte*)cmd);

    _MySleep(10);
    

    HWEDC_Data_0x31 strx31;
    memcpy(&strx31, RepData, sizeof(strx31));
    memcpy(gCCardNo, &strx31.field_14, 30);

    if (rtn == SUCCEED)
    {
         _Strprintf(slog, "UTEDCAPICALL_T707() ==> rtn=%d , Card_No:%30.30s...", rtn, gCCardNo);
    }
    else
    {  
        _Strprintf(slog, "UTEDCAPICALL_T707() ==> rtn=%d ErrorCode:%s, ErrorMsg:%s...", rtn, CommEDC->ucHostResponseCode, CommEDC->sTsccApiErrorMsg.c_str() );
    }
   
    delete CommEDC;
    wsEdcMsg= _StringToWString(slog); // ANSII String to Unicode

    return(rtn);
}

//// FETC 存在
int UTEDCAPICALL_T708(int PayAmt)
{
    int iTranType, iTMode, iAmt, rtn, iTranNo;
    string slog, sDttm, sBandCode, sCard_no;
    char InData[1024], RepData[1024], cmd[5];

    //MessageBox(NULL, L"UTEDCAPICALL_T708() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);

    rtn = 0; iTranType = 708;  iTMode = 0;
    iAmt = PayAmt;
    sDttm = _currentDateTime(1, 0);
    iTranNo = _StrToInt(sDttm.substr(9, 6));
    sprintf_s(cmd, "FE01");


    sBandCode = "0001";       //(66,4);     2017/07/31
    sCard_no = "01234567890";       //2018/05/31


    sprintf_s(InData, "%6s%-8s%08d%6s%10s%-14s%-6s%4s%04d%4s%-30s",
        "009939", "01", iTranNo, "999999", "XX01234567", "0", "0", cmd, 4, sBandCode.c_str(), sCard_no.c_str());

    cLsEDC* CommEDC = new cLsEDC();

    rtn = CommEDC->EdcApiCall(iTranType, iTMode, (byte*)InData, (byte*)RepData, iAmt, (byte*)cmd);

    _MySleep(10);

    if (rtn == SUCCEED)
    {
        _Strprintf(slog, "UTEDCAPICALL_T708() ==> rtn=%d ...", rtn);
    }
    else
    {
        _Strprintf(slog, "UTEDCAPICALL_T708() ==> rtn=%d ErrorCode:%s, ErrorMsg:%s...", rtn, CommEDC->ucHostResponseCode, CommEDC->sTsccApiErrorMsg.c_str());
    }

    delete CommEDC;
    wsEdcMsg = _StringToWString(slog); // ANSII String to Unicode

    //MessageBox(NULL, L"UTEDCAPICALL_T708() EDD ....", L"Msg title", MB_OK | MB_ICONQUESTION);

    return(rtn);
     
}

// FETC 結帳
int UTEDCAPICALL_T709(int PayAmt)
{
    int iTranType, iTMode, iAmt, rtn, iTranNo;
    string slog, sDttm, sBandCode, sCard_no;
    char InData[1024], RepData[1024], cmd[5];

    //MessageBox(NULL, L"UTEDCAPICALL_T709() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);

    rtn = 0; iTranType = 709;  iTMode = 0;
    iAmt = PayAmt;
    sDttm = _currentDateTime(1, 0);
    iTranNo = _StrToInt(sDttm.substr(9, 6));
    sprintf_s(cmd, "0000");

    sBandCode = "0000";             //(66,4);     2017/07/31
    sCard_no = "01234567890";       //2018/05/31

    sprintf_s(InData, "%6s%-8s%08d%6s%10s%-14s%-6s%4s%04d%4s%-30s",
        "009939", "01", iTranNo, "999999", "XX01234567", sDttm.c_str(), "0", cmd, 4, sBandCode.c_str(), sCard_no.c_str());

    cLsEDC* CommEDC = new cLsEDC();

    rtn = CommEDC->EdcApiCall(iTranType, iTMode, (byte*)InData, (byte*)RepData, iAmt, (byte*)cmd);

    _MySleep(10);

    if (rtn == SUCCEED)
    {
        _Strprintf(slog, "UTEDCAPICALL_T709() ==> rtn=%d ...", rtn);
    }
    else
    {
        _Strprintf(slog, "UTEDCAPICALL_T709() ==> rtn=%d ErrorCode:%s, ErrorMsg:%s...", rtn, CommEDC->ucHostResponseCode, CommEDC->sTsccApiErrorMsg.c_str());
    }

    delete CommEDC;
    wsEdcMsg = _StringToWString(slog); // ANSII String to Unicode
    //MessageBox(NULL, L"UTEDCAPICALL_T709() END ....", L"Msg title", MB_OK | MB_ICONQUESTION);
    return(rtn);
    
}

int UTEDCAPICALL_SIG_T710(int PayAmt)
{
    //MessageBox(NULL, L"UTEDCAPICALL_SIG_T710() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
    UTEDCAPICALL_T707(PayAmt);
    UTEDCAPICALL_T704(PayAmt);
    Sleep(1000);
    UTEDCAPICALL_SIG_T711(PayAmt);
    //_Strprintf(slog, "UTEDCAPICALL_T707() ==> rtn=%d , Card_No:%30.30s...", rtn, gCCardNo);
    //MessageBox(NULL, _StringToWString(slog).c_str(), L"Msg title", MB_OK | MB_ICONQUESTION);
    //MessageBox(NULL, L"UTEDCAPICALL_SIG_T710() END ....", L"Msg title", MB_OK | MB_ICONQUESTION);

    return(0);
}


int UTEDCAPICALL_SIG_T711(int PayAmt)
{
    int iTranType, iTMode, iAmt, rtn, iTranNo;
    string slog;  // sDttm;
    char InData[1024], RepData[1024], cmd[5];

    //MessageBox(NULL, L"UTEDCAPICALL_SIG_T711() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
    //memset(gCCardNo, 0, sizeof(gCCardNo));

    rtn = 0; iTranType = 711;  iTMode = 0;
    iAmt = PayAmt;
   
    //sDttm = _currentDateTime(1, 0);
    //iTranNo = _StrToInt(sDttm.substr(8, 6));

    sprintf_s(InData, "S");
    sprintf_s(cmd, "0000");
    cLsEDC* CommEDC = new cLsEDC();

    rtn = CommEDC->EdcApiCall(iTranType, iTMode, (byte*)InData, (byte*)RepData, iAmt, (byte*)cmd);

    _MySleep(10);
    if (rtn == SUCCEED)
    {
        _Strprintf(slog, "UTEDCAPICALL_SIG_T711 ==> rtn=%d ...", rtn);
    }
    else
    {
        _Strprintf(slog, "UTEDCAPICALL_SIG_T711 ==> rtn=%d ErrorCode:%s, ErrorMsg:%s...", rtn, CommEDC->ucHostResponseCode, CommEDC->sTsccApiErrorMsg.c_str());
    }

    delete CommEDC;
    wsEdcMsg = _StringToWString(slog); // ANSII String to Unicode

    //_Strprintf(slog, "UTEDCAPICALL_T711() END ==> rtn=%d ...", rtn);
    //MessageBox(NULL, _StringToWString(slog).c_str(), L"Msg title", MB_OK | MB_ICONQUESTION);
    return(rtn);

}




int UTEDCAPICALL_DBtn3(int PayAmt)
{   
    wstring wslog;
    int iRtn;
    //MessageBox(NULL, L"UTEDCAPICALL_SIG_T710() Start ....", L"Msg title", MB_OK | MB_ICONQUESTION);
    wslog = _T("★ Start UTEDCAPICALL_T707() ★");
    AddString(NULL, wslog);
    iRtn=UTEDCAPICALL_T707(PayAmt);

    if (iRtn != SUCCEED)
    {
        wslog = _T("★ Start UTEDCAPICALL_T707() Error:") + wsEdcMsg;
    }
    else
    {
        wslog = _T("★ Start UTEDCAPICALL_T707() OK :") + wsEdcMsg;
    }
    AddString(NULL, wslog);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    wslog = _T("★ Start UTEDCAPICALL_T704() ★");
    AddString(NULL, wslog);
    iRtn = UTEDCAPICALL_T704(PayAmt);
    if (iRtn != SUCCEED)
    {
        wslog = _T("★ Start UTEDCAPICALL_T704() Error:") + wsEdcMsg;
    }
    else
    {
        wslog = _T("★ Start UTEDCAPICALL_T704() OK :") + wsEdcMsg;
    }
    AddString(NULL, wslog);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (PayAmt > 3000 && iRtn == SUCCEED)
    {
        wslog = _T("★ Start UTEDCAPICALL_T711() ★");
        AddString(NULL, wslog);
        Sleep(1000);



        iRtn = UTEDCAPICALL_SIG_T711(PayAmt);
        if (iRtn != SUCCEED)
        {
            wslog = _T("★ Start UTEDCAPICALL_T711() Error:") + wsEdcMsg;
        }
        else
        {
            wslog = _T("★ Start UTEDCAPICALL_T711() OK :") + wsEdcMsg;
        }
        AddString(NULL, wslog);
    }

    //_Strprintf(slog, "UTEDCAPICALL_T707() ==> rtn=%d , Card_No:%30.30s...", rtn, gCCardNo);
    //MessageBox(NULL, _StringToWString(slog).c_str(), L"Msg title", MB_OK | MB_ICONQUESTION);
    //MessageBox(NULL, L"UTEDCAPICALL_SIG_T710() END ....", L"Msg title", MB_OK | MB_ICONQUESTION);

    return(iRtn);
}





inline UINT InitCBComPortText()
{
    HWND hwd_CB;
    TCHAR ComPortNo[11][10] =
    {
        TEXT("0"), TEXT("01"), TEXT("02"), TEXT("03"),
        TEXT("04"), TEXT("05"), TEXT("06"), TEXT("07"),
        TEXT("08"),TEXT("09"),TEXT("10")
    };


    //if (hList == NULL)
        hwd_CB = GetDlgItem(hwd_IDD_FORMPPAGE, IDC_COMBO1);
    //else
    //    hwd_CB = hList;


        TCHAR A[16];
        int  k = 0;

        memset(&A, 0, sizeof(A));

        //SendMessage(hwd_CB, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)TEXT("Item 1") );
        /***/
        for (k = 0; k < 11; k += 1)
        {
            //wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)ComPortNo[k]);  //Buge

            // Add string to combobox.
            SendMessage(hwd_CB, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)ComPortNo[k]);
        }
        /****/  
        // Send the CB_SETCURSEL message to display an initial item 
        //  in the selection field  
        SendMessage(hwd_CB, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);



    //return static_cast<UINT>(SendMessage(hwd_CB, LB_RESETCONTENT, 0, 0));
        return 0;

}





/**************
void DoEvents()
{
    MSG msg;
    BOOL result;

    while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
    {
        result = ::GetMessage(&msg, NULL, 0, 0);
        if (result == 0) // WM_QUIT
        {
            ::PostQuitMessage(msg.wParam);
            break;
        }
        else if (result == -1)
        {
            // Handle errors/exit application, etc.
        }
        else
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }
}




*********************/
