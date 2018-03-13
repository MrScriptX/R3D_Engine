#pragma once

#include "VkHeader.h"
#include "Buffer.h"
#include <stdexcept>

class DescriptorSet
{
public:
	DescriptorSet(VkDevice const& device, VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorPool& descriptorPool, VkBuffer& buffer, VkImageView& texImageView, VkSampler& sampler);

	void bindDescriptorSet(std::vector<VkCommandBuffer>& commandBuffer, VkPipelineLayout& pipelineLayout);

	VkDescriptorSet& get();
private:

	VkDescriptorSet m_descriptorSet;
};

