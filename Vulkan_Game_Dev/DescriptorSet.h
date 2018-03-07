#pragma once

#include "VkHeader.h"
#include "Buffer.h"
#include <stdexcept>

class DescriptorSet
{
public:
	DescriptorSet(VkDevice const& device, VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorPool& descriptorPool, Buffer& buffer);

	VkDescriptorSet& get();
private:

	VkDescriptorSet m_descriptorSet;
};

