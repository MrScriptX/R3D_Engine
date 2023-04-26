#ifndef VRED_RENDERER_INTERFACE_H
#define VRED_RENDERER_INTERFACE_H

#include <vulkan/vulkan.h>

namespace vred::renderer
{
	struct interface
	{
	    VkInstance instance = VK_NULL_HANDLE;
	    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
	    VkSurfaceKHR surface = VK_NULL_HANDLE;
	};
}

#endif // !VRED_RENDERER_INTERFACE_H
