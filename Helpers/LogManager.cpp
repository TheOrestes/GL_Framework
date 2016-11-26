
#include "LogManager.h"
#include "Helper.h"
#include "../Dependencies/termcolor/include/termcolor.hpp"

//////////////////////////////////////////////////////////////////////////
LogManager::LogManager()
{
}

//////////////////////////////////////////////////////////////////////////
void LogManager::WriteToConsole( LOGTYPE type, const std::string& file, const std::string& message )
{
	std::string filename = Helper::ExtractRawFileName(file);

	switch (type)
	{
	case LOG_INFO:
	{
		std::cout << termcolor::cyan << " INFO\t" << CurrentDateTime().c_str() << "\t" << file.c_str() << "\t" << message.c_str() << std::endl;
		break;
	}
		
	case LOG_ERROR:
	{
		std::cout << termcolor::red << " ERROR\t" << CurrentDateTime().c_str() << " \t" << file.c_str() << "\t" << message.c_str() << std::endl;
		break;
	}
		
	case LOG_RAW:
	{
		std::cout << termcolor::white << "" << message.c_str() << std::endl;
		break;
	}

	case LOG_DEBUG:
	{
		std::cout << termcolor::yellow << " DEBUG\t" << CurrentDateTime().c_str() << "\t" << file.c_str() << "\t" << message.c_str() << std::endl;
		break;
	}

	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
const std::string LogManager::CurrentDateTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];

	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d  %X", &tstruct);

	return buf;
}

//////////////////////////////////////////////////////////////////////////
LogManager::~LogManager()
{

}




