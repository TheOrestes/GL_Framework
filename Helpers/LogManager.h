
#pragma once

#include <iostream>
#include <time.h>

enum LOGTYPE 
{
	LOG_RAW,
	LOG_INFO,
	LOG_DEBUG,
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

	// http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
	const std::string CurrentDateTime();
	void	WriteToConsole(LOGTYPE type, const std::string& file, const std::string& message);

private:
	LogManager();
};
