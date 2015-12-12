#include "StdAfx.h"
#include "LogFile.h"


LogFile::LogFile(const std::wstring& logfile)                               
{
	m_logFile = logfile;
}

LogFile::~LogFile(void)
{
}

void LogFile::parseA()                                                   
{
	std::ifstream ifs(m_logFile.c_str());                                 
	if(!ifs.is_open())
		return;

	std::string lastLine;
	while(!ifs.eof())
	{
		std::string line;
		std::getline(ifs, line);
		if( !isLineHead(line))
		{
			if(!lastLine.empty())                                       
				lastLine+= line;
		}
		else
		{
			LineStruct ls;
			parseLine(lastLine, ls);                                       ///-------first line is empty

			if(!ls.Time.empty())          
			m_lines.push_back(ls);
			lastLine = line;
			
		}

		//process last line
		if(ifs.eof())
		{
			LineStruct ls;
			parseLine(lastLine, ls);
			m_lines.push_back(ls);
			
			
			break;
		}
	}
}

void LogFile::parseLine(const std::string& lineStr, LineStruct& ls)
{

	if (lineStr.empty())
	{
		return;
	}
	size_t pos1,pos2,pos3,pos4;
	pos1= lineStr.find(' ');	
/*
	if (std::string::npos==pos1)
		{
			return;
		}*/
	pos2=lineStr.find('t=');
	pos3=lineStr.find(' ',pos2);
	pos4=lineStr.find(':',pos3);
	ls.Time=lineStr.substr(0,pos1);
	ls.ThreadId=lineStr.substr(pos2+1,pos3-pos2);
	ls.Level=lineStr.substr(pos3,pos4-pos3);
	ls.LogString =lineStr.substr(pos4+1,lineStr.npos-pos4);	

}

bool LogFile::isLineHead( const std::string& lineStr )
{


		if (lineStr.size()>9)
	
		{
				if((lineStr.at(2)==':')&&(lineStr.at(5)==':'))
				{
				   if(isdigit(lineStr.at(0))&&isdigit(lineStr.at(1))&&isdigit(lineStr.at(3))&&isdigit(lineStr.at(4)))
					{
		                return true;       
					}			      
				}		
		 }	
		
	
		
    return false;
}

