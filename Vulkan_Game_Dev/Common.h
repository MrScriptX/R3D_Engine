#pragma once


#define GLFW_INCLUDED_VULKAN
#include <GLFW\glfw3.h>

#include <vulkan\vulkan.h>

#include <memory>
#include <vector>

namespace Renderer
{
	struct Vulkan
	{
		GLFWwindow* window = nullptr;
		VkSurfaceKHR surface = VK_NULL_HANDLE;

		VkInstance instance = VK_NULL_HANDLE;
		VkDevice device = VK_NULL_HANDLE;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		uint32_t graphics_queue_family_index = 0;
		uint32_t present_queue_family_index = 0;
		VkQueue graphics_queue = VK_NULL_HANDLE;
		VkQueue present_queue = VK_NULL_HANDLE;
	};

	struct ValidationLayer
	{
		const std::vector<const char*> validation_layer = { "VK_LAYER_LUNARG_standard_validation" };

#ifdef NDEBUG
		const bool enable_validation_layer = false;
#else
		const bool enable_validation_layer = true;
#endif // !NDEBUG
	};
}