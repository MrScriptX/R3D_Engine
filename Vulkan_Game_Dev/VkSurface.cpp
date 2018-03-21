#include "VkSurface.h"



VkSurface::VkSurface(VkInstance& instance, GLFWwindow& window) : m_instance(instance)
{
	if (glfwCreateWindowSurface(m_instance, &window, nullptr, &m_surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}


VkSurface::~VkSurface()
{
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
}

VkSurfaceKHR& VkSurface::getSurface()
{
	return m_surface;
}
