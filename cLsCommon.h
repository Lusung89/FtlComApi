


#pragma once

//#ifndef CLSCOMMONH
//#define CLSCOMMONH
#include <stdio.h>

#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/stat.h>  

#include <algorithm>
#include <atlstr.h> 
#include <tchar.h>
#include <string>
//#include <cstring>
#include <iomanip>
#include "Shlwapi.h"
#include <sstream> 
#include <fstream>
#include <vector>
#include <iterator>
//#include <experimental/filesystem> // or #include <filesystem>

#include <list>
#include <iostream>
#include "Shlwapi.h"
#include "Shlobj.h"
#include <time.h>
#include "INI.h"

//#include <locale.h>
//#include <mbstring.h>

using namespace std;
//using namespace std::transform;

typedef enum FileType
{
	TypeANSI = 0,
	TypeUNICODE = 1,
	TypeUTF8 = 2,
}  FILETYPE;

string  __stdcall _RTrim(const string& str);
string  __stdcall _LTrim(const string& str);
string  __stdcall _Trim(const string& str);
string  __stdcall _Trim(const LPSTR  str);
string  __stdcall  _stringToUpper(string oString);
string  __stdcall  _capitalizeString(string s);

string  __stdcall _int2str(int i);
string __stdcall  logStr2Hex(LPSTR  pStrData, int pStrDataLength);
int     __stdcall _StrToInt(string S);
string __stdcall _WStringToString(const std::wstring& wstr); // Unicode to ANSII String
wstring __stdcall _StringToWString(const std::string& str); // ANSII String to Unicode
vector <wstring> __stdcall _WStringSegment_VT(string str, string delimiter, int seg, int idelSt);



//template< typename T >
//string __stdcall _IntToHex(T i);
string __stdcall _IntToHex(int i);
string __stdcall _StrToHex(string sour);
string __stdcall _HexToStr(string sour);


string __stdcall _TcharToString(TCHAR* pTstr);
bool __stdcall _StrFind(const std::string& str, const std::string& sfind);
int __stdcall _Pos(const std::string& str, const std::string& sfind);
string __stdcall _Strprintf(std::string& str, const char* format, ...);

#pragma region   FileSystem Function
//  if (PathFileExists( _StringToWString(EDCSIGFILE).c_str() )  )    //電簽檔
int __stdcall    _GetFileList(TStringList* lsFileList, std::wstring refcstrRootDirectory, std::wstring refFileExt = L"*.*", int iBefDay = 0, int ifileCnt = 0);
//int __stdcall    _DeleteDirectory(std::wstring& refcstrRootDirectory, bool bDeleteSubdirectories = true);
int __stdcall    _DeleteDirectory(wstring refcstrRootDirectory, bool bDeleteSubdirectories = true);

bool __stdcall _DirExists(const std::string& dirName_in);
void __stdcall _DelTree(string dirName_in);
void __stdcall _wMtnDirFile_Ex(wstring wdirName_in, int MtnDay, int FileCnt);
void __stdcall _MtnDirFile_Ex(string dirName_in, int MtnDay, int FileCnt);
void __stdcall WriteDllFile32(string TmpFileName, char* pstr, int strlen);


//變更副檔名
string __stdcall _ChangeFileExt(const std::string& str, string& newExt);
// 取得檔案不含路徑 
string __stdcall _ExtractFile(string str);
// 取得檔案路徑 
string __stdcall _ExtractFilePath(string str);
string  __stdcall _currentDateTime(int iDttmTypw, int OffsetDay);


void __stdcall   _WOpenFile(wstring FileName, wstring MEcode);
wstring __stdcall _ReadOneLine(FILE* File, wstring Line);
FILETYPE __stdcall _GetTextFileType(const std::string& strFileName);
std::stringstream __stdcall _AnsiFileTostringstream(std::string filename);
std::string __stdcall _AnsiFileTostring(std::string filename);

int __stdcall _UnicodeToANSI(char* pDes, const wchar_t* wpSrc);
string __stdcall _StringToUTF8(const std::string& str);
string __stdcall _UTF8Tostring(const std::string& str);
wstring __stdcall _UTF8ToWUnicode(const std::string& sUtf8);
string __stdcall _UnicodeToUTF8(const wstring& str);
BOOL __stdcall IsBig5(const char* pstr);
BOOL __stdcall IsUTF8(const char* pstr);

void __stdcall _Sleep(int islp);
void __stdcall _DoEvents();

#pragma endregion