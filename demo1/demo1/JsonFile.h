#pragma once
#include <fstream>
#include "json\\json.h"
class JsonFile
{
public:
	JsonFile(void);
	~JsonFile(void);
	Json::Value OpenJson(HWND &hwnd);
};
