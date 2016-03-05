
#include "LogManager.h"
#include "../Dependencies/TermColor/termcolor.hpp"

//////////////////////////////////////////////////////////////////////////
LogManager::LogManager()
{
}

//////////////////////////////////////////////////////////////////////////
void LogManager::WriteToConsole( LOGTYPE type, const std::string& message )
{
	switch (type)
	{
	case LOG_INFO:
		{
			std::cout << termcolor::cyan <<  ">> INFO : " << message.c_str() << std::endl;
		}
		break;

	case LOG_ERROR:
		{
			std::cout << termcolor::red << ">> ERROR : " << message.c_str() << std::endl;
		}
		break;

	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
LogManager::~LogManager()
{

}




