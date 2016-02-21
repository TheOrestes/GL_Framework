
// Windows Only!! - https://www.daniweb.com/programming/software-development/code/216345/add-a-little-color-to-your-console-text

#pragma once

#include <Windows.h>   // WinApi header
#include <iostream>

enum LOGTYPE 
{
	LOG_INFO,
	LOG_ERROR,
	LOG_MAX
};

class LogManager
{
public:
	static LogManager& getInstance()
	{
		static LogManager instance;
		return instance;
	}

	~LogManager();

	void	WriteToConsole(LOGTYPE type, const std::string& message);

private:
	LogManager();
};
