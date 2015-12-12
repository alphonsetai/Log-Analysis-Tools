#pragma once

#include "string"
#include "list"
#include <fstream>

class LogFile
{
public:
	struct LineStruct                              
	{
		std::string Time;
		std::string ThreadId;
		std::string Level;
		std::string LogString;                        
	};

public:
	LogFile(const std::wstring& logfile);
	~LogFile(void);

	void parseA();
//	

//private:
	void parseLine(const std::string& lineStr, LineStruct& ls);
	bool isLineHead(const std::string& lineStr);
	const std::list<LineStruct>& GetLines()        
	{                               
		return m_lines;
	};


private:
	std::wstring m_logFile;
	std::list<LineStruct> m_lines;
};



