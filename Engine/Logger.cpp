#include "Includes/Logger.h"


Logger::Logger()
{
}


Logger::~Logger()
{
}

void Logger::init()
{
	std::fstream log;

	try
	{
		log.open("Log/log.txt", std::fstream::out | std::fstream::trunc);
		log.close();
	}
	catch (std::ios_base::iostate& e)
	{
		throw std::runtime_error("Log.txt could not be opened");
	}
}

void Logger::registerError(std::string errorMsg)
{
	std::fstream log;

	try
	{
		log.open("Log/log.txt", std::fstream::out | std::fstream::ate | std::fstream::app);

		std::time_t tmp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		log << std::ctime(&tmp) + ' ' + errorMsg;

		log.close();
	}
	catch(std::ios_base::iostate& e)
	{
		throw std::runtime_error("Log.txt could not be used!");
	}
}
