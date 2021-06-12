// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the FTLCOMAPI_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// FTLCOMAPI_API functions as being imported from a DLL, whereas this DLL sees symbols

#include <stdio.h>
#include <tchar.h>
#include "cLsEDC.h"

using namespace std;


// defined with this macro as being exported.
#ifdef FTLCOMAPI_EXPORTS
#define FTLCOMAPI_API __declspec(dllexport)
#else
#define FTLCOMAPI_API __declspec(dllimport)
#endif

// This class is exported from the dll
class FTLCOMAPI_API CFtlComApi {
public:
	CFtlComApi(void);
	~CFtlComApi(void);
	// TODO: add your methods here.
};


extern "C" FTLCOMAPI_API int nFtlComApi;

//How to Build FTLCOMAPI_API for windows XP
//[Project] => [Properities] => [Configuration Properities] => [General] => [Platform ToolSet] ==>  "Visual Studio 2013 - Windows XP (v120_xp)"
//[Platform ToolSet] ==> (Default:)"Visual Studio 2013 (V120)
/*
extern "C" XZDATAAPI_API int __stdcall fnXZdataApi_INIT(void);
extern "C" XZDATAAPI_API int __stdcall INITXZDATA(byte * OrgStoreNO, byte * StoreNO, byte * EcrNO, byte * Version);
extern "C" XZDATAAPI_API int __stdcall WX(byte * StoreNO, byte * EcrNO, byte * SalesNo, byte * SalFileName, byte * Version);
extern "C" XZDATAAPI_API int __stdcall WZ(byte * StoreNO, byte * EcrNO, byte * SalesNo, byte * SalFileName, byte * Version, int AutoZ);
extern "C" XZDATAAPI_API int __stdcall WXReport(byte * StoreNO, byte * EcrNO, byte * SalesNo, byte * SalFileName, byte * Version);
extern "C" XZDATAAPI_API int __stdcall Checkin(byte * StoreNO, byte * EcrNO, byte * SalesNo, byte * SalFileName, byte * Version);
extern "C" XZDATAAPI_API int __stdcall WX_OK(byte * StoreNO, byte * EcrNO, byte * SalesNo, byte * SalFileName, byte * Version);
extern "C" XZDATAAPI_API int __stdcall WZ_OK(byte * StoreNO, byte * EcrNO, byte * SalesNo, byte * SalFileName, byte * Version, bool delFolder);
extern "C" XZDATAAPI_API int __stdcall WSPC(byte * StoreNO, byte * EcrNO, byte * SalesNo, byte * SalFileName, byte * Version, int iQryType);
extern "C" XZDATAAPI_API int __stdcall WVXZ(byte * StoreNO, byte * EcrNO, byte * SalesNo, byte * SalFileName, byte * Version);
*/

extern "C" FTLCOMAPI_API int __stdcall fnFtlComApi(void);
extern "C" FTLCOMAPI_API int __stdcall FtlComApiInit(void);
extern "C" FTLCOMAPI_API int __stdcall FtlComApiTest(int TranType, int TMode, byte * InData, byte * OutData, int iAmt, byte * Cmd);
extern "C" FTLCOMAPI_API int __stdcall FtlComApiGetRep(int TranType, int TMode, byte * RepData, byte * OutData, int &OutDataLen);
extern "C" FTLCOMAPI_API int __stdcall FtlComApi(int TranType, int TMode, byte * InData, byte * OutData, int iAmt, byte * Cmd);
