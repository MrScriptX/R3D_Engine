#include "vcommandbuffer.h"

#include <stdexcept>

VkCommandPool vred::renderer::create_command_pool(const VkDevice& device, uint32_t queue_family_index)
{
	VkCommandPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.pNext = nullptr;
	pool_info.queueFamilyIndex = queue_family_index;
	pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VkCommandPool command_pool;
	if (vkCreateCommandPool(device, &pool_info, nullptr, &command_pool) != VK_SUCCESS)
		throw std::runtime_error("failed to create command pool!");

	return command_pool;
}

std::vector<VkCommandBuffer> vred::renderer::allocate_command_buffers(uint32_t count, const VkDevice& device, const VkCommandPool& command_pool)
{
	VkCommandBufferAllocateInfo alloc_buffers_info = {};
	alloc_buffers_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_buffers_info.commandPool = command_pool;
	alloc_buffers_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_buffers_info.commandBufferCount = count;

	std::vector<VkCommandBuffer> command_buffers(count);
	if (vkAllocateCommandBuffers(device, &alloc_buffers_info, command_buffers.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers!");

	return command_buffers;
}
