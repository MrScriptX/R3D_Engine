#include "DescriptorSet.h"

#include <array>

DescriptorSet::DescriptorSet(VkDevice const& device, VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorPool& descriptorPool, VkBuffer& buffer, VkImageView& texImageView, VkSampler& sampler)
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

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = texImageView;
	imageInfo.sampler = sampler;

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	std::array<VkWriteDescriptorSet, 2> descriptorWrite = {};
	descriptorWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite[0].dstSet = m_descriptorSet;
	descriptorWrite[0].dstBinding = 0;
	descriptorWrite[0].dstArrayElement = 0;
	descriptorWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite[0].descriptorCount = 1;
	descriptorWrite[0].pBufferInfo = &bufferInfo;

	descriptorWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite[1].dstSet = m_descriptorSet;
	descriptorWrite[1].dstBinding = 1;
	descriptorWrite[1].dstArrayElement = 0;
	descriptorWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite[1].descriptorCount = 1;
	descriptorWrite[1].pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrite.size()), descriptorWrite.data(), 0, nullptr);
}

VkDescriptorSet& DescriptorSet::get()
{
	return m_descriptorSet;
}
