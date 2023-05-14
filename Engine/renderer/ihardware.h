#ifndef VRED_RENDERER_IHARDWARE_H
#define VRED_RENDERER_IHARDWARE_H

#include <vulkan/vulkan.h>

#include "queue_family.h"

namespace vred::renderer
{
	struct ihardware
	{
	    VkInstance instance = VK_NULL_HANDLE;
	    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
		VkDevice device = VK_NULL_HANDLE;
	    VkSurfaceKHR surface = VK_NULL_HANDLE;

		queue_family_indices queue_indices;
		VkQueue graphics_queue = VK_NULL_HANDLE;
		VkQueue present_queue = VK_NULL_HANDLE;
	};
}

#endif // !VRED_RENDERER_IHARDWARE_H
