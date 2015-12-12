#include "StdAfx.h"
#include "JsonFile.h"

JsonFile::JsonFile(void)
{
}

JsonFile::~JsonFile(void)
{
}

//����json�ļ�
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
		MessageBox(NULL,_T("Json�ļ���ʧ�ܻ�ȱ�ٱ�Ҫ��Jash�ļ�!"),_T("ERROR"),MB_OK|MB_ICONWARNING);return 0;
	}  
	Json::Value root;  
	Json::Reader reader;  
	if (!reader.parse(openjson, root, false))  
	{  
		MessageBox(NULL,_T("Json�ļ����ô���!"),_T("ERROR"),MB_OK|MB_ICONWARNING);return 0;
	}  
return root;
}  

