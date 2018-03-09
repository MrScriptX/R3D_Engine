#include "DescriptorPool.h"

#include <stdexcept>
#include <array>


DescriptorPool::DescriptorPool(VkDevice const& device) : m_device(device)
{
	std::array<VkDescriptorPoolSize, 2> poolSize = {};
	poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize[0].descriptorCount = 1;
	poolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSize[1].descriptorCount = 1;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());
	poolInfo.pPoolSizes = poolSize.data();
	poolInfo.maxSets = 1;

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}


DescriptorPool::~DescriptorPool()
{
	vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);

	std::clog << "DescriptorPool destroyed\n";
}

VkDescriptorPool & DescriptorPool::getDescriptor()
{
	return m_descriptorPool;
}
