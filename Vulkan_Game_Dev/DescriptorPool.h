#pragma once

#include "VkHeader.h"

class DescriptorPool
{
public:
	DescriptorPool();
	~DescriptorPool();

	void createDescriptorPool(VkDevice& device);
	void clean(VkDevice& device);

	VkDescriptorPool& getDescriptor();
private:
	VkDescriptorPool m_descriptorPool;
};

