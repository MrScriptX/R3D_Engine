#include "DescriptorPool.h"

#include <stdexcept>


DescriptorPool::DescriptorPool(VkDevice const& device) : m_device(device)
{
	VkDescriptorPoolSize poolSize = {};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = 1;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = 1;

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}


DescriptorPool::~DescriptorPool()
{
	vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);
}

VkDescriptorPool & DescriptorPool::getDescriptor()
{
	return m_descriptorPool;
}
