#pragma once

#include "VkHeader.h"
#include "Buffer.h"
#include <stdexcept>

class DescriptorSet
{
public:
	DescriptorSet();
	~DescriptorSet();

	void createDescriptorSet(VkDevice& device, VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorPool& descriptorPool, Buffer& buffer);

	VkDescriptorSet& getDescriptor();
private:
	VkDescriptorSet m_descriptorSet;
};

