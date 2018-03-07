#include "CommandPool.h"



CommandPool::CommandPool(VkDevice const& device, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface) : m_device(device)
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

	VkCommandPoolCreateInfo commandPoolInfo = {};
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
	commandPoolInfo.flags = 0;

	if (vkCreateCommandPool(m_device, &commandPoolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}


CommandPool::~CommandPool()
{
	vkDestroyCommandPool(m_device, m_commandPool, nullptr);

	std::clog << "CommandPool destroyed\n";
}

VkCommandPool & CommandPool::get()
{
	return m_commandPool;
}
