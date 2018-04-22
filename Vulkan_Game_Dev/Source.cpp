#include "Application.h"
#include <iostream>
#include <stdexcept>


int main()
{
	Application app;

	try
	{
		app.run();
	}
	catch(const std::runtime_error& error)
	{
		std::cerr << error.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}