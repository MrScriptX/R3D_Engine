#pragma once

#ifndef COMMON_INCLUDED

#define GLFW_INCLUDED_VULKAN
#include <GLFW\glfw3.h>
#include <vulkan\vulkan.h>

#define COMMON_INCLUDED

#endif // !COMMON_INCLUDED

#include <memory>
#include <vector>

namespace Renderer
{
	struct Vulkan
	{
		GLFWwindow* window = nullptr;

		VkInstance instance = VK_NULL_HANDLE;

		VkDevice device = VK_NULL_HANDLE;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
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