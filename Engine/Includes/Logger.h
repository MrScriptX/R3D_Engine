#ifndef _LOGGER_H
#define _LOGGER_H

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

class Logger
{
  public:
	Logger();
	~Logger();

	static void init();
	static void registerError(std::string errorMsg);
};

#endif //!_LOGGER_H
