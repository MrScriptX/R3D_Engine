#ifndef VRED_RENDERER_VBUFFER_H
#define VRED_RENDERER_VBUFFER_H

#include <vulkan/vulkan.h>

#include "ihardware.h"

namespace vred::renderer
{
	VkBuffer create_buffer(const ihardware& hw, VkDeviceSize size, VkBufferUsageFlags usage);
	VkDeviceMemory allocate_buffer(const ihardware& hw, const VkBuffer& buffer, VkMemoryPropertyFlags properties);
}

#endif // !VRED_RENDERER_VBUFFER_H
