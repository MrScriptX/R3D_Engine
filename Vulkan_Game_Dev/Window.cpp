#include "Window.h"



Window::Window(Renderer::Vulkan& vulkan) : m_vulkan(vulkan)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_vulkan.window = glfwCreateWindow(1920, 1080, "Vulkan", nullptr, nullptr);

	glfwSetWindowUserPointer(m_vulkan.window, nullptr);
}


Window::~Window()
{
}
