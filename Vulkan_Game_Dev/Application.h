#pragma once

#include "Window.h"
#include "Instance.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:

	void setup();
	void gameLoop();

	Renderer::Vulkan m_vulkan;
	std::unique_ptr<Window> m_cWindow;
	std::unique_ptr<Instance> m_cInstance;
};

