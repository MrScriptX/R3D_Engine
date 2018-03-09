#pragma once

#include "VkHeader.h"
#include "Buffer.h"
#include <stdexcept>

class DescriptorSet
{
public:
	DescriptorSet(VkDevice const& device, VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorPool& descriptorPool, VkBuffer& buffer, VkImageView& texImageView, VkSampler& sampler);

	VkDescriptorSet& get();
private:

	VkDescriptorSet m_descriptorSet;
};

