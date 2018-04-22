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
	quit();
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

void Application::quit()
{
		if (enableValidationLayers)
		{
			m_cInstance->DestroyDebugReportCallbackEXT(m_vulkan.instance, m_cInstance->getCallback(), nullptr);
		}

		vkDestroyInstance(m_vulkan.instance, nullptr);

		glfwDestroyWindow(m_vulkan.window);

		glfwTerminate();
}
