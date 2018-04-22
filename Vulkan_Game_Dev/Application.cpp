#include "Application.h"



Application::Application()
{
}


Application::~Application()
{
}

void Application::run()
{
	setup();
	gameLoop();
}

void Application::setup()
{
	m_cWindow = std::make_unique<Window>(m_vulkan);
	m_cInstance = std::make_unique<Instance>(m_vulkan);
}

void Application::gameLoop()
{
	while (!glfwWindowShouldClose(m_vulkan.window))
	{
		glfwPollEvents();
	}
}