#include "../Includes/renderer/VulkanDescriptor.h"

VulkanDescriptor::VulkanDescriptor(Graphics& graphic) : m_graphic(graphic)
{
}

VulkanDescriptor::~VulkanDescriptor()
{
}

void VulkanDescriptor::createDescriptorPool()
{
	std::array<VkDescriptorPoolSize, 2> pool_sizes = {};
	pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pool_sizes[0].descriptorCount = 1;
	pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	pool_sizes[1].descriptorCount = 1;

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
	pool_info.pPoolSizes = pool_sizes.data();
	pool_info.maxSets = 100;

	if (vkCreateDescriptorPool(m_graphic.device, &pool_info, nullptr, &m_graphic.descriptor_pool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void VulkanDescriptor::createDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layout_info = {};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
	layout_info.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(m_graphic.device, &layout_info, nullptr, &m_graphic.descriptor_set_layout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void VulkanDescriptor::createDescriptorSetLayoutLight()
{
	VkDescriptorSetLayoutBinding ubo_binding = {};
	ubo_binding.binding = 0;
	ubo_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	ubo_binding.descriptorCount = 1;
	ubo_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	ubo_binding.pImmutableSamplers = nullptr;

	std::array<VkDescriptorSetLayoutBinding, 1> bindings = { ubo_binding };
	VkDescriptorSetLayoutCreateInfo layout_info = {};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
	layout_info.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(m_graphic.device, &layout_info, nullptr, &m_graphic.light_descriptor_layout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor set layout !");
	}
}

void VulkanDescriptor::createDescriptorSet(const VkDescriptorPool& descriptor_pool, VkDescriptorSetLayout& layout, VkDescriptorSet& descriptor_set)
{
	VkDescriptorSetLayout layouts[] = { layout };
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptor_pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts;

	VkResult result = vkAllocateDescriptorSets(m_graphic.device, &allocInfo, &descriptor_set);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate descriptor set!");
	}
}

void VulkanDescriptor::bindDescriptorSet(VkCommandBuffer& commandBuffer, VkPipelineLayout& pipelineLayout, VkDescriptorSet& descriptorSet)
{
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
}
