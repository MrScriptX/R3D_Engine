#ifndef VRED_RENDERER_VCOMMAND_BUFFER_H
#define VRED_RENDERER_VCOMMAND_BUFFER_H

#include <vulkan/vulkan.h>
#include <vector>

namespace vred::renderer
{
	VkCommandPool create_command_pool(const VkDevice& device, uint32_t queue_family_index);
	std::vector<VkCommandBuffer> allocate_command_buffers(uint32_t count, const VkDevice& device, const VkCommandPool& command_pool);
}

#endif // !VRED_RENDERER_VCOMMAND_BUFFER_H
