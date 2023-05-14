#ifndef VRED_RENDERER_HELPER_H
#define VRED_RENDERER_HELPER_H

#include <vulkan/vulkan.h>

namespace vred::renderer
{
	uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& physical_device);
}

#endif // !VRED_RENDERER_HELPER_H
