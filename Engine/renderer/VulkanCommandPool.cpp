#include "..\Includes\renderer\VulkanCommandPool.h"



VulkanCommandPool::VulkanCommandPool(Graphics& m_graphic) : m_graphic(m_graphic)
{
	createCommandPool();
}


VulkanCommandPool::~VulkanCommandPool()
{
}

void VulkanCommandPool::createCommandPool()
{
	VkCommandPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.pNext = nullptr;
	pool_info.queueFamilyIndex = m_graphic.queue_indices.graphic_family;
	pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (vkCreateCommandPool(m_graphic.device, &pool_info, nullptr, &m_graphic.command_pool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}
