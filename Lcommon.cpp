#include "stdafx.h"
#include "Lcommon.h"

string LlogFileName;
string LWorkDir;
string LlogDir;

const int FBLOCK_MAX_BYTES = 256;    // File Type.  

/// <summary>  Lcommon_init()
///  Init Lcommon ..Create sWorkDir目錄, Create sLogFile 檔案.. 程式啟動時執行.
/// </summary>
/// <param name="sWorkDir"></param>
/// <param name="sLogFile"></param>
/// <returns>Void</returns>
void __stdcall Lcommon_init(string sWorkDir, string sLogFile)
{
	
	if (sWorkDir.empty())
		sWorkDir = "c:\\FtlPos\\";

	LWorkDir = sWorkDir;
	LlogDir = sWorkDir + "log\\";

	LlogFileName = LlogDir + sLogFile;

	wstring wstmp = StringToWString(sWorkDir);
	//建立目錄  LWorkDir
	if (!DirExists(sWorkDir))
		SHCreateDirectory(NULL, wstmp.c_str());

	wstmp = StringToWString(LlogDir);
	//建立目錄 LlogDir
	if (!DirExists(LlogDir))
		SHCreateDirectory(NULL, wstmp.c_str());

}



/// <summary> wMtnDirFile_Ex
/// 刪除 sDir 目錄下的檔案
/// </summary>
/// <param name="wdirName_in"></param>
/// <param name="MtnDa"></param>
/// <param name="FileCnt"></param>
/// <returns>void</returns>
void __stdcall _wMtnDirFile_Ex(wstring wdirName_in, int MtnDay, int FileCnt)
{

	wstring mtn_file, FileExt;
	string stmp, str;

	TStringList *tsl = new TStringList();

	stmp = WStringToString(wdirName_in);
	logsprintf("MtnDirFile_Ex 刪除 %s 目錄下,%d天前的檔案", stmp.c_str(), MtnDay);

	tsl->clear();
	_GetFileList(tsl, wdirName_in);

	int icnt = 0;
	//for (int i = 0; i<tsl->Count; i++)
	for (list<string>::iterator ls = tsl->begin(); ls != tsl->end(); ls++)
	{
		str = *ls;
		//mtn_file = _ExtractFile(str);
		//FileExt = ExtractFileExt(mtn_file);

		DeleteFile(StringToWString(str).c_str()); // sDir + mtn_file);
		logsprintf("MtnDirFile_Ex:%d DeleteFile(%s%s)", icnt, str.c_str()); //, mtn_file.c_str());

		icnt++;
		if ((FileCnt > 0) && (icnt >= FileCnt))
			break;
	}

	delete tsl;

}

/// <summary> GetFileList
/// List refcstrRootDirectory 目錄下, iBefDay 天前的檔案
/// </summary>
/// <param name="lsFileList"></param>
/// <param name="refcstrRootDirectory"></param>
/// <param name="refFileExt"></param>
/// <param name="iBefDay"></param>
/// <param name="ifileCnt"></param>
/// <returns>int</returns>

int __stdcall _GetFileList(TStringList *lsFileList, std::wstring &refcstrRootDirectory, std::wstring refFileExt, int iBefDay, int ifileCnt)
{
	HANDLE          hFile;                       // Handle to directory
	std::wstring    strFilePath;                 // Filepath
	std::wstring    strPattern;                  // Pattern
	std::wstring    strCreateDttm;
	WIN32_FIND_DATA FileInformation;             // File information
	std::string     str, slog, strDt, strCrtDt;
	char            buf[80];

	int iCnt = 0;

	lsFileList->clear();
	str = _WStringToString(refcstrRootDirectory);

	if (str.substr(str.length() - 1, 1) != "\\")
	{
		str = str + "\\";
		refcstrRootDirectory = refcstrRootDirectory + L"\\";
	}

	if (!_DirExists(str))
	{
		writelog("★★GetFileList:【錯誤】目錄不存在!! -> " + str);
		return 0; 
	}

	SYSTEMTIME utc;
	time_t   result, now;
	struct tm  tstruct;
	strDt = "";

	if (iBefDay != 0)
	{
		now = time(0);
		localtime_s(&tstruct, &now);
		tstruct.tm_mday -= iBefDay;

		if ((result = mktime(&tstruct)) != (time_t)-1)
		{
			localtime_s(&tstruct, &result);
			strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);
			strDt = string(buf);     //比對日期
		}
		else
		{
			//localtime_s(&tstruct, &now);
			strDt = "";
			writelog("★★GetFileList:【錯誤】mktime error!!!! ");
			return 0;  
		}

	}
	
	strPattern = refcstrRootDirectory + refFileExt; // L"*.*";
	hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (FileInformation.cFileName[0] != '.')
			{
				strFilePath.erase();
				strFilePath = refcstrRootDirectory + FileInformation.cFileName;

				if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					;;
				}
				else
				{
					// Set file attributes
					//if (::SetFileAttributes(strFilePath.c_str(),
					//	FILE_ATTRIBUTE_NORMAL) == FALSE)
					//	return ::GetLastError();

					//// Delete file
					//if (::DeleteFile(strFilePath.c_str()) == FALSE)
					//	return ::GetLastError();
										
					FileTimeToSystemTime(std::addressof(FileInformation.ftCreationTime), std::addressof(utc));
					_Strsprintf(strCrtDt, "%04d%02d%02d", utc.wYear, utc.wMonth, utc.wDay);
					str = _WStringToString(strFilePath);

					if (!strDt.empty() && strDt.length() == 8)
					{
						if (strCrtDt.compare(strDt) >= 0)
						{
							lsFileList->push_back(str);
							_Strsprintf(slog, "push_back:File:%s, -> %d-%d-%d Week=%d", str.c_str(), utc.wYear, utc.wMonth, utc.wDay, utc.wDayOfWeek);
							iCnt++;
						}

					}
					else
					{
						
						lsFileList->push_back(str);
						_Strsprintf(slog, "push_back:File:%s", str.c_str());
						iCnt++;

					}
					
					if (ifileCnt > 0 && iCnt >= ifileCnt)  // 取得一定數量檔案數
						break;
				}
			}
		} while (::FindNextFile(hFile, &FileInformation) == TRUE);
	}

	return(iCnt);

}

/// <summary> DeleteDirectory
/// 刪除 refcstrRootDirectory 目錄下的檔案, 包含Sub目錄
/// </summary>
/// <param name="refcstrRootDirectory"></param>
/// <param name=" bDeleteSubdirectories"></param>
/// <returns>int</returns>
int __stdcall _DeleteDirectory(std::wstring &refcstrRootDirectory, bool bDeleteSubdirectories)
{
	bool  bSubdirectory = false;       // Flag, indicating whether
	// subdirectories have been found
	HANDLE          hFile;                       // Handle to directory
	std::wstring     strFilePath;                 // Filepath
	std::wstring     strPattern;                  // Pattern
	WIN32_FIND_DATA FileInformation;             // File information
	string str;

	str = _WStringToString(refcstrRootDirectory);

	if (str.substr(str.length() - 1, 1) != "\\")
	{
		str = str + "\\";
		refcstrRootDirectory = refcstrRootDirectory + L"\\";
	}

	if (!_DirExists(str))
	{
		writelog("★★DeleteDirectory:【錯誤】目錄不存在!! -> " + str);
		return 0;  // 要 #include "FileCtrl.hpp" 才有這個函式
	}

	refcstrRootDirectory = StringToWString(str);
	strPattern = refcstrRootDirectory + L"*.*";
	hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (FileInformation.cFileName[0] != '.')
			{
				strFilePath.erase();
				strFilePath = refcstrRootDirectory + FileInformation.cFileName;

				if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (bDeleteSubdirectories)
					{
						// Delete subdirectory
						int iRC = _DeleteDirectory(strFilePath, bDeleteSubdirectories);
						if (iRC)
							return iRC;
					}
					else
						bSubdirectory = true;
				}
				else
				{
					// Set file attributes
					if (::SetFileAttributes(strFilePath.c_str(),
						FILE_ATTRIBUTE_NORMAL) == FALSE)
						return ::GetLastError();

					// Delete file
					if (::DeleteFile(strFilePath.c_str()) == FALSE)
						return ::GetLastError();
				}
			}
		} while (::FindNextFile(hFile, &FileInformation) == TRUE);

		// Close handle
		::FindClose(hFile);

		DWORD dwError = ::GetLastError();
		if (dwError != ERROR_NO_MORE_FILES)
			return dwError;
		else
		{
			if (!bSubdirectory)
			{
				// Set directory attributes
				if (::SetFileAttributes(refcstrRootDirectory.c_str(),
					FILE_ATTRIBUTE_NORMAL) == FALSE)
					return ::GetLastError();

				// Delete directory
				if (::RemoveDirectory(refcstrRootDirectory.c_str()) == FALSE)
					return ::GetLastError();
			}
		}
	}

	writelog("★★DeleteDirectory:目錄 -> " + str);
	return 0;
}

/// <summary>  _DirExists
///  判斷目錄是否存在.  return true :目錄存在
/// </summary>
/// <param name="dirName_in"></param>
/// <returns>bool</returns>
bool __stdcall _DirExists(const std::string& dirName_in)
{
	int ftyp = _access(dirName_in.c_str(), 0);
	if (0 == ftyp)
		return true;   // this is a directory!  
	else
		return false;    // this is not a directory!  
}

/// <summary> _DelTree
/// 刪除 dir 目錄下的檔案, 包含Sub目錄
/// </summary>
/// <param name="dirName_in"></param>
/// <returns>Void</returns>
void __stdcall _DelTree(string dirName_in)
{
	_DeleteDirectory(StringToWString(dirName_in));  //用更名備份來代替刪目錄
	return;

}

/// <summary> MtnDirFile_Ex
/// 刪除 sDir 目錄下的檔案
/// </summary>
/// <param name="dirName_in"></param>
/// <param name="MtnDa"></param>
/// <param name="FileCnt"></param>
/// <returns>void</returns>
void __stdcall _MtnDirFile_Ex(string dirName_in, int MtnDay, int FileCnt)
{

	string mtn_file, FileExt, stmp;

	TStringList *tsl = new TStringList();

	logsprintf("MtnDirFile_Ex 刪除 %s 目錄下,%d天前的檔案", dirName_in.c_str(), MtnDay);

	tsl->clear();
	_GetFileList(tsl, StringToWString(dirName_in), L"*.*", MtnDay, FileCnt);
	
	int icnt = 0;
	//for (int i = 0; i<tsl->Count; i++)
	for (list<string>::iterator ls = tsl->begin(); ls != tsl->end(); ls++)
	{
		mtn_file = *ls; // ExtractFileName(tsl->Strings[i]);
		                // FileExt = ExtractFileExt(mtn_file);

		DeleteFile( StringToWString(mtn_file).c_str());
		logsprintf("MtnSalFile :%d DeleteFile(%s%s)", icnt, dirName_in.c_str(), mtn_file.c_str());
		writelog(stmp);

		icnt++;
		//if ((FileCnt > 0) && (icnt >= FileCnt))
		//	break;
	}

	delete tsl;

}

//變更副檔名
//string __stdcall _ChangeFileExt(const std::string& str, string& newExt)
//{
//	string s = str;
//
//	string::size_type i = s.rfind('.', s.length());
//
//	if (i != string::npos) {
//		s.replace(i, newExt.length(), newExt);
//	}
//
//	return s;
//}


// 取得檔案不含路徑 
//string __stdcall _ExtractFile(string str)
//{
//	std::wstring wStr = L"";
//	string sFileNameNonPath = "";
//	wStr = PathFindFileName(StringToWString(str).c_str());     //檔名不包含路徑
//	CW2A cw2a(wStr.c_str());
//	sFileNameNonPath = cw2a;
//	return sFileNameNonPath;
//}

// 取得檔案路徑 
//string __stdcall _ExtractFilePath(string str)
//{
//	string strrtn = "";
//	try
//	{
//		const size_t last_slash_idx = str.find_last_of('\\');
//		if (std::string::npos != last_slash_idx)
//		{
//			strrtn = str.substr(0, last_slash_idx) + "\\";
//		}
//	}
//	catch (...)
//	{
//		writelog("_ExtractFilePath: Catch error =>" + str);
//	}
//
//
//	/*DWORD  retval = 0;
//	TCHAR  buffer[1024] = TEXT("");
//	TCHAR** lppPart = { NULL };
//	retval = GetFullPathName(StringToWString(str).c_str(),
//	1024,
//	buffer,
//	lppPart);
//
//	if (retval == 0)
//	{
//	strrtn = "";
//	}
//	else
//	{
//
//	strrtn = TcharToString(buffer);
//	}*/
//
//	return(strrtn);
//
//}


/// <summary> _WOpenFile 
/// Open Unicode file , 
/// </summary>
/// <param name="FileName"></param>
/// <param name = "MEcode:r,ccs=UNICODE">< / param>
/// <param name="MEcode:r,ccs=UTF-16LE"></param>
/// <param name="MEcode:r,ccs=UTF-8"></param>  
/// <param name="MEcode:r,ccs=CP_ACP"></param> 
/// <returns> void</returns>
void __stdcall _WOpenFile(wstring FileName, wstring MEcode )
{
	//("堃；", "煊；"媖)  
	FILE *file;
	wstring line;
	errno_t err;

	err = _wfopen_s(&file, FileName.c_str(), MEcode.c_str());  //L"r,ccs=UTF-16LE", L"r,ccs=UTF-8"

	if (err == 0)
	{
		logsprintf("The file %s was opened ,MEcode=%s ", _WStringToString(FileName).c_str(), _WStringToString(MEcode).c_str());
	}
	else
	{
		logsprintf("The file %s  opene erroe=%d ", _WStringToString(FileName).c_str(), err);
	}


	while (!feof(file) && !ferror(file)){
		line = _ReadOneLine(file, line);
		//wcout << line;
	}

	fclose(file);
}


//系統顯示與Deault 是 Unicode, 所以 wstring 就是 Unicode 資料編碼.  //UTF8堃:\e5\a0\83
//如開檔是 ccs=UTF-8 , _ReadOneLine 會讀入 UTF-8 編碼之檔案, 但還是會轉成 wstring:Unicode 
wstring __stdcall _ReadOneLine(FILE *File, wstring Line)
{

	wchar_t LineOfChars[4096];
	wmemset(LineOfChars, 0x00, 4096);
	fgetws(LineOfChars, 4096, File);

	Line.clear();
	Line.append(LineOfChars);
	//string str = _WStringToString(Line);  // Unicode 轉成 Big5 後, ("堃；", "煊, "媖" ) 會無法顯示, 變成 "?"
	//str=_UnicodeToUTF8(Line);

	return Line;
}

//std::fstream <fstream.h> 有三個 功能 :
//ifstream : Read file
//ofstream : write file
//fstream  : Read/write file
std::stringstream __stdcall _AnsiFileTostringstream(std::string filename)
{
	std::stringstream dosString(std::ios::out | std::ios::binary); // *** binary
	std::ifstream inFile(filename.c_str());
	std::string line;

	dosString.clear();
	if (!inFile)     //檢查檔案是否成功開啟，如果!file為真，表示無法開啟檔案
	{
		while (std::getline(inFile, line)) dosString << line << "\r\n";
    	inFile.close();
	}

	return dosString;
}

std::string __stdcall _AnsiFileTostring(std::string filename)
{
	std::stringstream dosString(std::ios::out | std::ios::binary); 
	dosString = _AnsiFileTostringstream(filename);
	return dosString.str();
}


/// <summary> _GetTextFileType 
/// Open file and gey file type
/// TypeANSI = 0,
/// TypeUNICODE = 1,
/// TypeUTF8 = 2, 
/// </summary>
/// <param name="strFileName"></param>
/// <returns> FILETYPE </returns>
FILETYPE __stdcall _GetTextFileType(const std::string & strFileName)
{
	FILETYPE fileType = TypeANSI;
	std::ifstream file;
	file.open(strFileName.c_str(), std::ios_base::in);
	bool bUnicodeFile = false;
	if (file.good())
	{
		char szFlag[3] = { 0 };
		file.read(szFlag, sizeof(char) * 3);
		if ((unsigned char)szFlag[0] == 0xFF && (unsigned char)szFlag[1] == 0xFE)
		{
			fileType = TypeUNICODE;
		}
		else
			if ((unsigned char)szFlag[0] == 0xEF && (unsigned char)szFlag[1] == 0xBB && (unsigned char)szFlag[2] == 0xBF)
			{
				fileType = TypeUTF8;
			}
	}
	file.close();
	return fileType;
}



/// <summary> _NowDateTime 
/// Get Current DateTime
/// iDttmTypw = 0:YYYY-MM-DD HH:MM:SS, 1:YYYYMMDDHHMMSS, 2:YYYYMMDD, 3:HH:MM:SS, 4:YYYY-MM-DD
/// OffsetDay Current DateTime (+-) Date ,
/// </summary>
/// <param name="iDttmTypw"></param>
/// <param name="OffsetDay"></param>
/// <returns> string </returns>
string  __stdcall _NowDateTime(int iDttmTypw, int OffsetDay)
{
	time_t   result, now;
	struct tm  tstruct;
	char       buf[80];

	now = time(0);
	localtime_s(&tstruct, &now);


	if (OffsetDay != 0)
	{
		tstruct.tm_mday += OffsetDay;
		if ((result = mktime(&tstruct)) != (time_t)-1)
		{
			localtime_s(&tstruct, &result);
		}
		else
		{
			localtime_s(&tstruct, &now);
		}
	}

	// for more information about date/time format
	switch (iDttmTypw)
	{
	case 0:
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
		break;
	case 1:
		strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tstruct);  //%H 24小時制的小時 %M 十時製表示的分鐘數 %S 十進制的秒數
		break;
	case 2:
		strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);
		break;
	case 3:
		strftime(buf, sizeof(buf), "%X", &tstruct);
		break;
	case 4:
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
		break;

	default:
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

	}

	return buf;
	/******************

	struct tm when;
	time_t now, result;
	int    days;

	time( &now );
	when = *localtime( &now );
	printf( "Current time is %s\n", asctime( &when ) );
	printf( "How many days to look ahead: " );
	scanf( "%d", &days );

	when.tm_mday = when.tm_mday + days;
	if( (result = mktime( &when )) != (time_t)-1 )
	printf( "In %d days the time will be %s\n",
	days, asctime( &when ) );
	else
	perror( "mktime failed" );




	%a 星期幾的簡寫

	%A 星期幾的全稱

	%b 月分的簡寫

	%B 月份的全稱

	%c 標準的日期的時間串

	%C 年份的後兩位數字

	%d 十進製表示的每月的第幾天

	%D 月 / 天 / 年

	%e 在兩字符域中，十進製表示的每月的第幾天

	%F 年 - 月 - 日

	%g 年份的後兩位數字，使用基於周的年

	%G 年分，使用基於周的年

	%h 簡寫的月份名

	%H 24小時制的小時

	% I 12小時制的小時

	% j 十進製表示的每年的第幾天

	%m 十進製表示的月份

	%M 十時製表示的分鐘數

	%n 新行符

	%p 本地的AM或PM的等價顯示

	%r 12小時的時間

	% R 顯示小時和分鐘：hh : mm

	%S 十進制的秒數

	%t 水平製表符

	%T 顯示時分秒：hh : mm : ss

	%u 每週的第幾天，星期一為第一天 （值從0到6，星期一為0）

	%U 第年的第幾周，把星期日做為第一天（值從0到53）

	%V 每年的第幾周，使用基於周的年

	%w 十進製表示的星期幾（值從0到6，星期天為0）

	%W 每年的第幾周，把星期一做為第一天（值從0到53）

	%x 標準的日期串

	%X 標準的時間串

	%y 不帶世紀的十進制年份（值從0到99）

	%Y 帶世紀部分的十制年份

	%z，%Z 時區名稱，如果不能得到時區名稱則返回空字符。

	%% 百分號

	**************/

}






string  __stdcall _LTrim(const string& str)
{

	string rtns = str;
	try
	{
		rtns.erase(0, rtns.find_first_not_of(" "));
		//rtns = str.substr(str.find_first_not_of(" \n\r\t"));
	}
	catch (...)
	{
		rtns = str;
	}
	return rtns;
}

string  __stdcall _RTrim(const string& str)
{
	string rtns = str;
	try
	{
		//str.substr(0, str.find_last_not_of(" \n\r\t") + 1);
		//rtns = str;
		rtns.erase(rtns.find_last_not_of(" \n\r\t") + 1);
	}
	catch (...)
	{
		rtns = str;
	}
	return rtns;

}


string  __stdcall _Trim(const string& str)
{
	string lstr = str;
	if (str.empty() || str.length() <= 0)
		return str;
	else
		return LTrim(RTrim(lstr));
}


string  __stdcall _Trim(const LPSTR  Lpstr)
{
	return(string(Lpstr));
}

//---------------------------------------------------------------------------
//依照分隔符號,取出分隔符號前的字串中
// "AAAAAA,BBBBBB,CCCCCC,DDDDDD,EEEEE," ; Seg(1)=AAAAAA, ~ Seg(5)=EEEEE
/// <summary> _StringSegment_VT
///  分割字串 Str , 分割字串 delimiter: ..,|,...  不適用含 UTF8, Big5
/// </summary>
/// <param name="Str"></param>
/// <param name="delimiter"></param>
/// <param name="Seg"></param>
/// <param name="idelSt"></param>
/// <returns>vector</returns>
vector <string> __stdcall _StringSegment_VT(string str, string delimiter, int seg, int idelSt)
{
	vector<string> wres;
	wres.clear();
	if ((str.empty()) || (delimiter.empty()) || (seg<1))
		return  wres;

	string ws = str;
	string wdelimiter = delimiter;
	//size_t pos_start = 0, pos_end, delim_len = wdelimiter.length();
	string::size_type  pos_start = 0, pos_end, delim_len = wdelimiter.length();
	string wtoken;

	while ((pos_end = ws.find(wdelimiter, pos_start)) != wstring::npos) {
		wtoken = ws.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		wres.push_back(wtoken);
	}
	wres.push_back(ws.substr(pos_start));
	//wtoken = wres[Seg - 1];
	//return (WStringToString(wtoken));

	return (wres);

}

//---------------------------------------------------------------------------
//依照分隔符號,取出分隔符號前的字串中
// "AAAAAA,BBBBBB,CCCCCC,DDDDDD,EEEEE," ; Seg(1)=AAAAAA, ~ Seg(5)=EEEEE
/// <summary> _WStringSegment_VT
///  分割字串 Str , 分割字串 delimiter: ..,|,...  適用含 UTF8, Big5
/// </summary>
/// <param name="Str"></param>
/// <param name="delimiter"></param>
/// <param name="Seg"></param>
/// <param name="idelSt"></param>
/// <returns>vector</returns>
vector <wstring> __stdcall _WStringSegment_VT(string str, string delimiter, int seg, int idelSt)
{
	vector<wstring> wres;
	wres.clear();
	if ((str.empty()) || (delimiter.empty()) || (seg<1))
		return  wres;

	wstring ws = StringToWString(str);
	wstring wdelimiter = StringToWString(delimiter);
	//size_t pos_start = 0, pos_end, delim_len = wdelimiter.length();
	wstring::size_type  pos_start = 0, pos_end, delim_len = wdelimiter.length();
	wstring wtoken;

	while ((pos_end = ws.find(wdelimiter, pos_start)) != wstring::npos) {
		wtoken = ws.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		wres.push_back(wtoken);
	}
	wres.push_back(ws.substr(pos_start));
	//wtoken = wres[Seg - 1];
	//return (WStringToString(wtoken));
	/*if (Seg - Stidx >= wres.size() || wres.size() == 0 )
		wtoken = L"";
	else
		wtoken = wres[Seg - Stidx];*/
	return (wres);

}

string __stdcall _TcharToString(TCHAR *pTstr)
{
	std::wstring wStr = pTstr;
	std::string str = std::string(wStr.begin(), wStr.end());
	return str;

}


//---------------------------------------------------------------------------
/// <summary> _StrPos
///  _StrPos 同 _StrFind 字串比對, 適用含 UTF8, Big5
/// </summary>
/// <param name="str"></param>
/// <param name="sfind"></param>
/// <returns>bool</returns>
int __stdcall _StrPos(const std::string &str, const std::string &sfind)
{
	std::size_t found;
	//WideCharToMultiByte();
	found = str.find(sfind);

	if (str.find(sfind) == std::string::npos)
		found = 0;
	return found;
}


string __stdcall _WStringToString(const std::wstring &wstr)
{
	std::string str;
	CW2A cw2a(wstr.c_str());
	str = cw2a;
	return str;
}



/// <summary> _StringToWString
///  string to unicode , 適用含 UTF8, Big5
/// </summary>
/// <param name="s"></param>
/// <returns>wstring</returns>
wstring __stdcall _StringToWString(const std::string &str)
{
	//fun1  無法對中文進行轉換, 因為是以 Byte ->  Unicode
	//std::wstring wsTmp(s.begin(), s.end());

	//fun2 Word To Unicode
	CA2W ca2w(str.c_str());
	wstring wsTmp = ca2w;
	return wsTmp;

}


// Code page ID FOR WideCharToMultiByte/ca2w  function
// CP_ACP: The system default Windows ANSI code page
// CP_THREAD_ACP: Windows 2000: The Windows ANSI code page for the current thread. 
// CP_MACCP: The current system Macintosh code page.
// CP_OEMCP: The current system OEM code page. 
// CP_UTF8:  UTF-8. 

/// <summary> _StringToUTF8
///  string to UTF8 , 適用含 Big5
/// </summary>
/// <param name="s"></param>
/// <returns>wstring</returns>

//系統顯示與Deault 是 Unicode, 所以 wstring 就是 Unicode 資料編碼.  
//UTF8堃:\e5\a0\83  
//Unicode堃:22531
//big5堃:63
wstring __stdcall _UTF8ToWUnicode(const std::string &sUtf8)
{
	// function 1
	// error checking omitted for brevity
	/*char const *utf8string = sUtf8.c_str();
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, NULL, 0);
	if (len == 0) { return L""; }
	vector<wchar_t> utf16Buf(len);
	MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, &utf16Buf[0], utf16Buf.size());
	wstring retVal(&utf16Buf[0]);*/

    //function 2
	 CA2W ca2w(sUtf8.c_str(), CP_UTF8);  //將 UTF8 To Unicode(wstring)
	 wstring retVal = ca2w;
     return retVal;
	
}


string __stdcall _UnicodeToUTF8(const wstring &str)
{
	char*     pElementText;
	int    iTextLen;
	//unsigned char uchr[300];

	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_UTF8,	0,	str.c_str(), -1, NULL, 0, NULL,	NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	string strText = pElementText;
	//memcpy(uchr, pElementText, sizeof(pElementText));
	//_UTF8ToWUnicode(strText);
	delete[] pElementText;
	
	// _StringToWString(strText);

	return strText;
}



int __stdcall UnicodeToANSI(char * pDes, const wchar_t *wpSrc)
{
	/*assert(pDes != NULL);
	assert(pSrc != NULL);*/
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, wpSrc, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
	{
		return -1;
	}
	return ::WideCharToMultiByte(CP_ACP, 0, wpSrc, -1, pDes, nLen, NULL, NULL);
}

string __stdcall string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然?出?尾巴  
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	// _UTF8ToWUnicode(retStr); 驗證

	return retStr;
}


string __stdcall UTF8_To_string(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然?出?尾巴  
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}




//判斷Big5
//Big5 第一位元編碼範圍是 0xa1 - 0xf9，第二位元範圍是 0x40 - 0x7e 與 0xa1 - 0xfe，
//不過這個範圍內仍有不存在任何字的地方
//
//依倚天中文系統系統字型檔的定義
//STDFONT.15 (共 13094 字)
//A440～C67E ← 5401 個常用字
//C940～F9D5 ← 7652 個次常用字
//C9D6～F9FE ← 41 cialis buy cialis online 個倚天字
//
//SPCFONT.15 (共 408 字)
//A140～A17E(63 個) A1A1～A1FE(94 個)
//A240～A27E(63 個) A2A1～A2FE(94 個)
//A340～A37E(63 個) A3A1～A3BF(31 個)
//
//SPCFSUPP.15 (共 408 字)
//C6A1～C6FE(94 個)
//C740～C77E(63 個) C7A1～C7FE(94 個)
//C840～C87E(63 個) C8A1～C8FE(94 個)
//
//用C來判斷就是

/// <summary> IsBig5
///  IsBig5 , 適用含 Big5
/// </summary>
/// <param name="char *p"></param>
/// <returns>BOOL</returns>
BOOL __stdcall IsBig5(const char *pstr)
{
	unsigned char a;
	unsigned char b;
	a = *pstr;
	b = *(pstr + 1);
	return (((a >= 0xA4 && a <= 0xC6) || (a >= 0xC9 && a <= 0xF9)) &&
		((b >= 0x40 && b <= 0x7E) || (b >= 0xA1 && b <= 0xFE)));
}




//判斷字串是否為utf8編碼
//0000 0080-0000 07FF - 110xxxxx 10xxxxxx    ( 2 octet format)
//0000 0800-0000 FFFF - 1110xxxx 10xxxxxx 10xxxxxx (3 octet format)
/// <summary> IsUTF8
///  IsUTF8 , 適用含 UTF8
/// </summary>
/// <param name="char *str"></param>
/// <returns>BOOL</returns>
BOOL __stdcall IsUTF8(const char *pstr)
{
	int   i;
	BYTE cOctets;  // octets to go in this UTF-8 encoded character
	BYTE chr;
	BOOL  bAllAscii = TRUE;
	long iLen = strlen(pstr);

	cOctets = 0;
	for (i = 0; i <iLen; i++)
	{
		chr = (BYTE)pstr[i];

		if ((chr & 0x80) != 0) bAllAscii = FALSE;

		if (cOctets == 0)
		{
			if (chr >= 0x80)
			{
				do  {
					chr <<= 1;
					cOctets++;
				} while ((chr & 0x80) != 0);
				cOctets--;
				if (cOctets == 0) return FALSE;
			}
		}
		else
		{
			if ((chr & 0xC0) != 0x80)
				return FALSE;
			cOctets--;
		}
	}

	if (cOctets> 0)
		return FALSE;

	if (bAllAscii)
		return FALSE;

	return TRUE;
}





string __stdcall _stringsprintf(std::string &str, char* format, ...)
{
	char buffer[10240];
	va_list argList;

	va_start(argList, format);
	vsnprintf_s(buffer, sizeof(buffer), format, argList);
	va_end(argList);

	str = string(buffer);
	return(str);
}


void  __stdcall _Llog(LPSTR pstr)
{
	writelog(string(pstr));
}


//LlogDir = sWorkDir + "log\\";
//LlogFileName = LlogDir + sLogFile;
void  __stdcall _Llog(string SLog)       //記錄LOG
{
	string str_now, stmp;
	str_now = currentDateTime(0); // FormatDateTime("yyyy-mm-dd hh:nn:ss", Now());
	fstream file;
	string str_ecr;
	string str_file;
	try
	{
		if (LlogFileName.empty())
		{
			str_ecr = "Lcommon"; //   sprintf("XZDATA"); //, atoi(gchar_ecr_no));
			str_file = LlogDir + str_ecr + ".log";
		}
		else
		{
			str_file = LlogFileName;
		}
		

		file.open(str_file.c_str(), ios::out | ios::app);
		if (file)
		{
			file << str_now.c_str() << " " << SLog.c_str() << "\n";
		}

	}
	catch (...)
	{
		;;
	}

	file.close();
}

void __stdcall _LlogPrintf(char* format, ...)
{
	char buffer[10240];
	va_list argList;

	va_start(argList, format);
	vsnprintf_s(buffer, 10240, format, argList);
	va_end(argList);

	writelog(buffer);
}





int __stdcall TestWVXZ(byte *StoreNO, byte *EcrNO, byte *SalesNo, byte *SalFileName, byte *Version)
{
	string strStoreNO = string((char*)StoreNO);
	string strEcrNO = string((char*)EcrNO);
	string strSalesNo = string((char*)SalesNo);
	string strSalFileName = string((char*)SalFileName);
	string strVersion = string((char*)Version);

	return 0;
}
