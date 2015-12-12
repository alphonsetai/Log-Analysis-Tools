#pragma once
#include"string"

class ToolFile
{
public:
	ToolFile(void);
	~ToolFile(void);
	std::wstring AnsiToUnicode(const char* lpcszString);
	std::wstring UTF8ToUnicode( const std::string &str) ;
};
