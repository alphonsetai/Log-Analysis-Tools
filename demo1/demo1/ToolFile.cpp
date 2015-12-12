#include "StdAfx.h"
#include "ToolFile.h"

ToolFile::ToolFile(void)
{
}

ToolFile::~ToolFile(void)
{
}


//*************************************************************************************************
//ANSIתunicode
std::wstring ToolFile::AnsiToUnicode(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete [] pUnicode;
	return wString;
}
//*****************************************************************************
//UTF8->UNICODE
std::wstring ToolFile::UTF8ToUnicode( const std::string &str)                                  
	{
		int  unicodeLen = ::MultiByteToWideChar( CP_UTF8,0,str.c_str(),-1,NULL,0 );  
		wchar_t *  pUnicode;  
		pUnicode = new  wchar_t[unicodeLen+1];  
		memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));  
		::MultiByteToWideChar( CP_UTF8,0,str.c_str(),-1,(LPWSTR)pUnicode,unicodeLen );  
		std::wstring  rt;  
		rt = ( wchar_t* )pUnicode;
		delete  pUnicode; 
		return  rt;  
	}