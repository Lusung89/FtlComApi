// IniFile.cpp: implementation of the CIniReader class.
//
//////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "stdafx.h"
#include "INI.h"
//#include <afxcoll.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/// <summary>  CIniReader::StringToWString
///   string to WString
/// </summary>
/// <param name="s"></param>
/// <returns>wstring</returns>
wstring CIniReader::StringToWString(const std::string &s)
{
	//fun1
	std::wstring wsTmp(s.begin(), s.end());

	//fun2
	//CA2W ca2w(s.c_str());
	//wstring wsTmp = ca2w;
	return wsTmp;

}

string CIniReader::WStringToString(const std::wstring &s)
{
	std::string str;
	CW2A cw2a(s.c_str());
	str = cw2a;
	return str;
}


string CIniReader::TcharToString(TCHAR *pTstr)
{
	std::wstring wStr = pTstr;
	std::string str = WStringToString(wStr); //  std::string(wStr.begin(), wStr.end()); not for unicode
	return str;

}

// Used to retrieve a value give the section and key
string CIniReader::getKeyValue(string strKey,string strSection)
{
	TCHAR ac_Result[255];

	// Get the info from the .ini file	
	//string s = "some string";
	//CA2W ca2w(s.c_str());  

	m_lRetValue = GetPrivateProfileString(StringToWString(strSection).c_str(), StringToWString(strKey).c_str(),
		L"", ac_Result, 255, StringToWString(m_strFileName).c_str());

	string strResult = TcharToString(ac_Result);
	return strResult;
}

// As BCB INI->ReadString
string CIniReader::ReadString(string strSection, string strKey, string strdefault)
{
	TCHAR ac_Result[255];

	// Get the info from the .ini file	
	//string s = "some string";
	//CA2W ca2w(s.c_str());  
	//uppercase()
	m_lRetValue = GetPrivateProfileString(StringToWString(strSection  ).c_str(), StringToWString(strKey).c_str(),
		L"", ac_Result, 255, StringToWString(m_strFileName).c_str());

	string strResult = TcharToString(ac_Result);
	if (strResult.length() <= 0 || strResult.empty() || _trim(strResult) == "")
	{
		strResult = _trim(strdefault);
		setKey(strResult, strKey, strSection);
	}
	return strResult;
}



string CIniReader::getKeyValue(string strKey, string strSection, string strdefault)
{
	TCHAR ac_Result[255];

	// Get the info from the .ini file	
	m_lRetValue = GetPrivateProfileString(StringToWString(strSection).c_str(), StringToWString(strKey).c_str(),
		L"", ac_Result, 255, StringToWString(m_strFileName).c_str());

	string strResult = TcharToString(ac_Result);
	if (strResult.length() <= 0 || strResult.empty() || _trim(strResult)=="" )
	{
		strResult = _trim(strdefault);
		setKey(strResult, strKey, strSection);
	}
	
	return strResult;
}

// Used to add or set a key value pair to a section
long CIniReader::setKey(string strValue, string strKey, string strSection)
{
	m_lRetValue = WritePrivateProfileString(StringToWString(strSection).c_str(), StringToWString(strKey).c_str(),
		StringToWString(strValue).c_str(), StringToWString(m_strFileName).c_str());

	return m_lRetValue;
}


long CIniReader::WriteString(string strSection, string strKey, string strValue)
{
	m_lRetValue = WritePrivateProfileString(StringToWString(strSection).c_str(), StringToWString(strKey).c_str(),
		StringToWString(strValue).c_str(), StringToWString(m_strFileName).c_str());

	return m_lRetValue;
}



// Used to find out if a given section exists
BOOL CIniReader::sectionExists(string strSection)
{
	TCHAR ac_Result[255];	
	string csAux;
	// Get the info from the .ini file	
	m_lRetValue = GetPrivateProfileString(StringToWString(strSection).c_str(), NULL,
		L"", ac_Result, 90, StringToWString(m_strFileName).c_str());
	// Return if we could retrieve any info from that section
	return (m_lRetValue > 0);
}

// Used to retrieve all of the  section names in the ini file
TStringList* CIniReader::getSectionNames()  //returns collection of section names
{
	CHAR ac_Result[4096];
	m_sectionList->clear();
	
	m_lRetValue = GetPrivateProfileSectionNamesA(ac_Result, 4096, m_strFileName.c_str());
	
	string strSectionName;
	for(int i=0; i<m_lRetValue; i++)
	{
		if(ac_Result[i] != '\0')
		{
			strSectionName += ac_Result[i];  
		} else {
			if(strSectionName != "") {
				m_sectionList->push_back(strSectionName);
			}
			strSectionName = "";
		}
	}

	return m_sectionList;
}

// Used to retrieve all key/value pairs of a given section.  
TStringList* CIniReader::getSectionData(string strSection)
{
	CHAR ac_Result[4096];  //change size depending on needs
	m_sectionDataList->clear();
	m_lRetValue = GetPrivateProfileSectionA(strSection.c_str(), ac_Result, 4096, m_strFileName.c_str());

	string strSectionData;
	for(int i=0; i<m_lRetValue; i++)
	{
		if(ac_Result[i] != '\0') 
		{
			strSectionData +=  ac_Result[i];
		} else {
			if(strSectionData != "") {
				m_sectionDataList->push_back( strSectionData  );
				

			}
			strSectionData = "";
		}
	}

	return m_sectionDataList;
}

void CIniReader::setINIFileName(string strINIFile)
{
	m_strFileName = strINIFile;
}


string CIniReader::_trim(string const& str)
{

	if (str.empty())
		return str;
	try
	{
		std::size_t firstScan = str.find_first_not_of(' ');
		std::size_t first = firstScan == std::string::npos ? str.length() : firstScan;
		std::size_t last = str.find_last_not_of(' ');
		return str.substr(first, last - first + 1);
	}
	catch (...)
	{ 
		;;;
	}


	/*std::string word;
	std::stringstream stream(str)
	stream >> word;
	return word;*/

	return "";

}
