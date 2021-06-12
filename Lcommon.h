#ifndef LCOMMONH
#define LCOMMONH   

#include "ULOG.h"

typedef enum FileType
{
	TypeANSI = 0,
	TypeUNICODE = 1,
	TypeUTF8 = 2,
}  FILETYPE;

#pragma region   Lcommon_init()
void __stdcall Lcommon_init(string sWorkDir, string sLogFile="");
#pragma endregion

#pragma region    Log  Function: before use Log function , call Lcommon_init();
void  __stdcall _Llog(LPSTR pstr);
void  __stdcall _Llog(string SLog);
void  __stdcall _LlogPrintf(char* format, ...);
#pragma endregion

#pragma region   FileSystem Function
int __stdcall    _GetFileList(TStringList *lsFileList, std::wstring &refcstrRootDirectory, std::wstring refFileExt = L"*.*", int iBefDay = 0, int ifileCnt = 0);
int __stdcall    _DeleteDirectory(std::wstring &refcstrRootDirectory, bool bDeleteSubdirectories = true);
bool __stdcall   _DirExists(const std::string& dirName_in);
void __stdcall   _DelTree(string dirName_in);
void __stdcall   _wMtnDirFile_Ex(wstring wdirName_in, int MtnDay, int FileCnt);
void __stdcall   _MtnDirFile_Ex(string dirName_in, int MtnDay, int FileCnt);
void __stdcall   _WOpenFile(wstring FileName, wstring MEcode);
wstring __stdcall _ReadOneLine(FILE *File, wstring Line);
FILETYPE __stdcall _GetTextFileType(const std::string & strFileName);
std::stringstream __stdcall _AnsiFileTostringstream(std::string filename);
std::string __stdcall _AnsiFileTostring(std::string filename);

//string __stdcall _ChangeFileExt(const std::string& str, string& newExt);
//string __stdcall _ExtractFile(string str);
//string __stdcall _ExtractFilePath(string str);
//int __stdcall file_cat(const char *sour, const char *targ)
#pragma endregion

#pragma region    Sal data Segment Function
string  __stdcall _LTrim(const string& str);
string  __stdcall _RTrim(const string& str);
string  __stdcall _Trim(const string& str);
string  __stdcall _Trim(const LPSTR  Lpstr);
vector <string>  __stdcall   _StringSegment_VT(string str, string delimiter, int seg, int idelSt = 0);
vector <wstring> __stdcall   _WStringSegment_VT(string str, string delimiter, int seg, int idelSt = 0);
string __stdcall _TcharToString(TCHAR *pTstr);
int __stdcall _StrPos(const std::string &str, const std::string &sfind);
string __stdcall _WStringToString(const std::wstring &wstr); // Unicode to ANSII String
wstring __stdcall _StringToWString(const std::string &str); // ANSII String to Unicode

int __stdcall UnicodeToANSI(char * pDes, const wchar_t *wpSrc);
string __stdcall string_To_UTF8(const std::string &str);
string __stdcall UTF8_To_string(const std::string &str);
wstring __stdcall _UTF8ToWUnicode(const std::string &sUtf8);
string __stdcall _UnicodeToUTF8(const wstring &str);
BOOL __stdcall IsBig5(const char *pstr);
BOOL __stdcall IsUTF8(const char *pstr);

string __stdcall _stringsprintf(std::string &str, char* format, ...);
#pragma endregion

#pragma region    DateTime  Function
string  __stdcall _NowDateTime(int iDttmTypw, int OffsetDay);
#pragma endregion

int __stdcall TestWVXZ(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version);



#endif
