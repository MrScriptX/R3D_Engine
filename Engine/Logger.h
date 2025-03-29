#ifndef VRED_LOGGER_H
#define VRED_LOGGER_H

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <mutex>

#include "log_type.h"

class Logger
{
  public:
	Logger();
	~Logger();

	static void init();
	static void registerError(const std::string& errorMsg);
};

namespace vred
{
	class Log
	{
	public:
		Log(const Log&) = delete;
		void operator=(const Log&) = delete;

		static void init();
		static Log& get();

		static void message(const LOG_TYPE& type, const std::string& message) noexcept;

		template<typename ...Args>
		static void message(const LOG_TYPE& type, const std::string& message, Args && ...args) noexcept;

	private:
		Log() = default;
		~Log() = default;

		std::mutex write_mutex;

		const std::string logfile = "log/engine.log";
	};


	template<typename ...Args>
	static void Log::message(const LOG_TYPE& type, const std::string& message, Args && ...args) noexcept
	{
		const std::string msg = std::vformat(message, std::make_format_args(args));
		Log::message(type, msg);
	}
}

#endif //!VRED_LOGGER_H
