#include "DescriptorSetLayout.h"



DescriptorSetLayout::DescriptorSetLayout()
{
}


DescriptorSetLayout::~DescriptorSetLayout()
{
}

void DescriptorSetLayout::createDescriptorSetLayout(VkDevice& device)
{
	VkDescriptorSetLayoutBinding uboBinding = {};
	uboBinding.binding = 0;
	uboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboBinding.descriptorCount = 1;
	uboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboBinding;

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void DescriptorSetLayout::clean(VkDevice& device)
{
	vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);
}

VkDescriptorSetLayout& DescriptorSetLayout::getDescriptor()
{
	return m_descriptorSetLayout;
}
