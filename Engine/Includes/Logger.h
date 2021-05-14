#ifndef _LOGGER_H
#define _LOGGER_H

#include <fstream>
#include <memory>
#include <string>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <iostream>

class Logger
{
public:
	Logger();
	~Logger();

	static void init();
	static void registerError(std::string errorMsg);
};

#endif //!_LOGGER_H