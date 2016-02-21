
#include "LogManager.h"

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
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << FOREGROUND_GREEN <<  ">> INFO : " << message.c_str() << std::endl;
		}
		break;

	case LOG_ERROR:
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << FOREGROUND_RED << ">> ERROR : " << message.c_str() << std::endl;
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




