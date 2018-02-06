#include "Init_Instance.h"



Init_Instance::Init_Instance()
{
}


Init_Instance::~Init_Instance()
{
}

void Init_Instance::run()
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanUp();
}

void Init_Instance::mainLoop()
{

}

void Init_Instance::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(1280, 720, "Vulkan", nullptr, nullptr);
}

void Init_Instance::initVulkan()
{

}

void Init_Instance::cleanUp()
{

}
