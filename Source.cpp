#include "Application.h"
#include "Engine.h"

int main()
{
	Application app;

	try
	{
		Logger::init();
		app.run();
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	return 0;
}