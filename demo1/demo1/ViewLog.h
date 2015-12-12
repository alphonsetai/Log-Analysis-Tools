#pragma once
#include "LogFile.h"
#include <Commctrl.h>
#include "algorithm"
#include "ToolFile.h"
class ViewLog
{
public:
	ViewLog(void);
	~ViewLog(void);
	void ShowListView(HWND &sE,HWND &hls,HWND &SI,int &pI,std::wstring &TS,LogFile *ll);
};
