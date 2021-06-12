// IniFile.h: interface for the CIniReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__99976B4B_DBA1_4D1E_AA14_CBEB63042FD1__INCLUDED_)
#define AFX_INIFILE_H__99976B4B_DBA1_4D1E_AA14_CBEB63042FD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <afxcoll.h>
#include <tchar.h>
#include <string>
#include <sstream> 
#include <fstream>
#include <list>
#include <iostream>
#include <atlbase.h>
#include <atlconv.h>


using namespace std;

typedef std::list<string> TStringList;  //for (list<string>::iterator s = Auto_1051_sal->begin(); s != Auto_1051_sal->end(); s++)

class CIniReader  
{
public:
	// method to set INI file name, if not already specified 
	void setINIFileName(string strINIFile);
	
	// methods to return the lists of section data and section names
	TStringList* getSectionData(string strSection);
	TStringList* getSectionNames();
	
	// check if the section exists in the file
	BOOL sectionExists(string strSection);

	// updates the key value, if key already exists, else creates a key-value pair
	long setKey(string strValue, string strKey, string strSection);
	long WriteString(string strSection, string strKey, string strValue );
	

	// give the key value for the specified key of a section
	string getKeyValue(string strKey,string strSection);
	string getKeyValue(string strKey, string strSection, string strdefault);
	string ReadString(string strSection, string strKey, string strdefault);
	
	// default constructor
	CIniReader()
	{
		m_sectionList = new TStringList();
		m_sectionDataList = new TStringList();
	}
	
	CIniReader(string strFile)
	{
		m_strFileName = strFile;
		m_sectionList = new TStringList();
		m_sectionDataList = new TStringList();
	}
	
	~CIniReader()
	{
		delete m_sectionList;
		delete m_sectionDataList;
	}
	
private:
	// lists to keep sections and section data
	TStringList *m_sectionDataList;
	TStringList *m_sectionList;
	
	string m_strSection;
	long m_lRetValue;
	
	// ini file name 
	string m_strFileName;
	string _trim(string const& str);
	wstring StringToWString(const std::string &s);
	string  WStringToString(const std::wstring &s);
	string  TcharToString(TCHAR *pTstr);
};

#endif // !defined(AFX_INIFILE_H__99976B4B_DBA1_4D1E_AA14_CBEB63042FD1__INCLUDED_)
