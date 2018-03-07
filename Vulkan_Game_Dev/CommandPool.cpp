#include "CommandPool.h"



CommandPool::CommandPool()
{
}


CommandPool::~CommandPool()
{
}

void CommandPool::createCommandPool(VkDevice& device, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface)
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

	VkCommandPoolCreateInfo commandPoolInfo = {};
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
	commandPoolInfo.flags = 0;

	if (vkCreateCommandPool(device, &commandPoolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

void CommandPool::clean(VkDevice & device)
{
	vkDestroyCommandPool(device, m_commandPool, nullptr);
}

VkCommandPool & CommandPool::get()
{
	return m_commandPool;
}
