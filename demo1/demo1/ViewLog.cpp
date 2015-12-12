#include "StdAfx.h"
#include "ViewLog.h"

ViewLog::ViewLog(void)
{
}

ViewLog::~ViewLog(void)
{
}

class Match																	//查找函数
{
public:
	Match(const std::string& SearchContent)
	{	
		sc= SearchContent; 
		//转换小写实现搜索时大小写忽略
		transform(sc.begin(),sc.end(),sc.begin(),tolower);  
	}
	bool operator ()(const LogFile::LineStruct& e)
	{
		std::string eve=e.Time+e.ThreadId+e.Level+e.LogString;
		//转换小写实现搜索时大小写忽略
		transform(eve.begin(),eve.end(),eve.begin(),tolower);
		return eve.find(sc)!=eve.npos;
	}
	std::string sc;
};
//**************************************************************************************************
//ListView显示过程
void ViewLog::ShowListView(HWND &sE,HWND &hls,HWND &SI,int &pI,std::wstring &TS,LogFile *ll)
{
	//Loading..
	SendMessage(SI,WM_SETTEXT,0,(LPARAM)TEXT("Loading.."));
	ToolFile *tf=NULL;
	//获得焦点
	SetFocus(sE);  
	SendMessage(hls, WM_SETREDRAW, 0, 0);                              
	ListView_DeleteAllItems(hls);
	int c=0;
	if (NULL != ll)
	{
		TCHAR InputBuff[128];
		GetWindowText(sE,InputBuff,128);
		std::wstring wSName=InputBuff;
		if (pI==9){wSName=TS;} 
		std::string SName(wSName.begin(),wSName.end());    			//wstring->string
		const std::list<LogFile::LineStruct>  &SearchName =ll->GetLines();
		std::list<LogFile::LineStruct>::const_iterator iter;
		iter=SearchName.begin();
        
		while((iter=std::find_if(iter,SearchName.end(), Match( SName)))!= SearchName.end())		
		{
			
			
//**********************************************************************************************
			//if (ic==0)
			if (pI==1)
			{
				if((iter->Level).find("INFO")==(iter->Level).npos)		
				{
					iter++;
					continue;	
				}
			}
			if (pI==2)
			{
				if((iter->Level).find("WARN")==(iter->Level).npos)		
				{
					iter++;
					continue;
				}
			}
			if (pI==3)
			{
				if((iter->Level).find("ERROR")==(iter->Level).npos)		
				{
					iter++;
					continue;
				}
			}
			
//*******************************************************************************************		
			std::wstring GetTimeStr=tf->UTF8ToUnicode(iter->Time);
			std::wstring GetThreadStr=tf->UTF8ToUnicode(iter->ThreadId);
			std::wstring GetLevelStr=tf->UTF8ToUnicode(iter->Level);	
			std::wstring GetLogStr=tf->UTF8ToUnicode(iter->LogString);

			LVITEM vitem={0};		
			vitem.mask = LVIF_TEXT;
			vitem.pszText =(LPTSTR)GetTimeStr.c_str();
			vitem.iItem = c;
			vitem.iSubItem=0;
			ListView_InsertItem(hls, &vitem);

			vitem.mask = LVIF_TEXT;
			vitem.pszText =(LPTSTR)GetThreadStr.c_str();				
			vitem.iSubItem=1;
			ListView_SetItem(hls,&vitem);

			vitem.mask = LVIF_TEXT;
			vitem.pszText =(LPTSTR)GetLevelStr.c_str();				
			vitem.iSubItem=2;
			ListView_SetItem(hls,&vitem);

			vitem.mask = LVIF_TEXT;
			vitem.pszText =(LPTSTR)GetLogStr.c_str();				
			vitem.iSubItem=3;
			ListView_SetItem(hls,&vitem);

			c++;
			iter++;
		}
		
		SendMessage(hls, WM_SETREDRAW, 1, 0);
		//ListView_GetItemCount
		int n=c;
		char num[256] ={0};
		_itoa_s(n,num,10);
		std::string str=num;
		std::wstring wstr(str.begin(),str.end());
		SendMessage(SI,WM_SETTEXT,0,(LPARAM)wstr.c_str());
	}
}