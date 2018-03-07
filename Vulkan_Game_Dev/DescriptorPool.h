#pragma once

#include "VkHeader.h"

class DescriptorPool
{
public:
	DescriptorPool(VkDevice const& device);
	~DescriptorPool();

	VkDescriptorPool& getDescriptor();
private:
	VkDescriptorPool m_descriptorPool;
	VkDevice const& m_device;
};

/*passer la description des descripteurs a allouer*/