#pragma once

#include "VkHeader.h"
#include <stdexcept>

class DescriptorSetLayout
{
public:
	DescriptorSetLayout();
	~DescriptorSetLayout();

	void createDescriptorSetLayout(VkDevice& device);
	void clean(VkDevice& device);

	VkDescriptorSetLayout& getDescriptor();

private:
	VkDescriptorSetLayout m_descriptorSetLayout;
};

