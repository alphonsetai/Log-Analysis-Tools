#include "StdAfx.h"
#include "JsonFile.h"

JsonFile::JsonFile(void)
{
}

JsonFile::~JsonFile(void)
{
}

//调用json文件
Json::Value OpenJson()
{
	TCHAR myPath[MAX_PATH];
	std::wstring szpath;
	std::string name,nameof;
	GetModuleFileName(NULL, myPath, MAX_PATH);
	(_tcsrchr(myPath, _T('\\')))[1] = 0;
	szpath=myPath;
	std::ifstream openjson(szpath.append(L"test.json").c_str());
	if (!openjson.is_open())  
	{  
		MessageBox(NULL,_T("Json文件打开失败或缺少必要的Jash文件!"),_T("ERROR"),MB_OK|MB_ICONWARNING);return 0;
	}  
	Json::Value root;  
	Json::Reader reader;  
	if (!reader.parse(openjson, root, false))  
	{  
		MessageBox(NULL,_T("Json文件配置错误!"),_T("ERROR"),MB_OK|MB_ICONWARNING);return 0;
	}  
return root;
}  

