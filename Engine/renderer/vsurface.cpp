#include "vsurface.h"

#include <stdexcept>

VkSurfaceKHR vred::renderer::create_surface(GLFWwindow& window, const VkInstance& instance)
{
	VkSurfaceKHR surface;
	if (glfwCreateWindowSurface(instance, &window, nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("Failed to create window surface!");

	return surface;
}
