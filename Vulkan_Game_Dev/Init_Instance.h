#pragma once


#include "vulkan.h"

class Init_Instance
{
public:
	Init_Instance();
	~Init_Instance();

	void run();
	void mainLoop();

	void initWindow();
	void initVulkan();

	void cleanUp();
private:
	GLFWwindow  * m_window = nullptr;
};

