#include "Logger.h"

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

void Logger::registerError(const std::string& errorMsg)
{
	std::fstream log;

	try
	{
		log.open("Log/log.txt", std::fstream::out | std::fstream::ate | std::fstream::app);

		std::time_t tmp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		log << std::ctime(&tmp) + ' ' + errorMsg;

		log.close();
	}
	catch (std::ios_base::iostate& e)
	{
		throw std::runtime_error("Log.txt could not be used!");
	}
}

void vred::Log::init()
{
	std::fstream log;

	try
	{
		log.open("log/engine.log", std::fstream::out | std::fstream::trunc);
		log.close();
	}
	catch (std::ios_base::iostate& e)
	{
		throw std::runtime_error("engine.log could not be opened");
	}
}

vred::Log& vred::Log::get()
{
	static vred::Log instance;
	return instance;
}

void vred::Log::message(const LOG_TYPE& type, const std::string& message) noexcept
{
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	const std::string log_msg = std::format("{} | {} | {}", type, std::ctime(&time), message);

	Log* log = &Log::get();

	std::fstream stream;
	stream.open(log->logfile, std::fstream::out | std::fstream::ate | std::fstream::app);

	log->write_mutex.lock();
	stream << log_msg;
	log->write_mutex.unlock();

	stream.close();
}
