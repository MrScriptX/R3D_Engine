#include "Surface.h"


Surface::Surface(Renderer::Vulkan& vulkan) : m_vulkan(vulkan)
{
	if (glfwCreateWindowSurface(m_vulkan.instance, &m_vulkan.window, nullptr, &m_vulkan.surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}


Surface::~Surface()
{
}
