#include "DescriptorSet.h"



DescriptorSet::DescriptorSet()
{
}


DescriptorSet::~DescriptorSet()
{
}

void DescriptorSet::createDescriptorSet(VkDevice& device, VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorPool& descriptorPool, Buffer& buffer)
{
	VkDescriptorSetLayout layouts[] = { descriptorSetLayout };
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts;

	if (vkAllocateDescriptorSets(device, &allocInfo, &m_descriptorSet))
	{
		throw std::runtime_error("failed to allocate descriptor set!");
	}


	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = buffer.getUniformBuffer();
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = m_descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
}

VkDescriptorSet& DescriptorSet::getDescriptor()
{
	return m_descriptorSet;
}
