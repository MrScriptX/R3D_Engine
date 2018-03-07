#pragma once

#include "VkHeader.h"
#include <stdexcept>

class DescriptorSetLayout
{
public:
	DescriptorSetLayout(VkDevice const& device);
	~DescriptorSetLayout();

	VkDescriptorSetLayout& get();

private:
	VkDevice const& m_device;
	VkDescriptorSetLayout m_descriptorSetLayout;
};

