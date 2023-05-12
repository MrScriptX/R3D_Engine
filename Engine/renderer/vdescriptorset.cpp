#include "vdescriptorset.h"

#include <stdexcept>

VkDescriptorSetLayoutBinding vred::renderer::create_descriptor_set_layout_binding(VkDescriptorType type, VkShaderStageFlags flags, uint32_t count, uint32_t binding)
{
	VkDescriptorSetLayoutBinding layout_binding = {};
	layout_binding.binding = binding;
	layout_binding.descriptorType = type;
	layout_binding.descriptorCount = count;
	layout_binding.stageFlags = flags;
	layout_binding.pImmutableSamplers = nullptr;

	return layout_binding;
}

VkDescriptorSetLayout vred::renderer::create_descriptor_set_layout(const VkDevice& device, const std::vector<VkDescriptorSetLayoutBinding>& bindings)
{
	VkDescriptorSetLayoutCreateInfo layout_info = {};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
	layout_info.pBindings = bindings.data();

	VkDescriptorSetLayout descriptor_set_layout;
	if (vkCreateDescriptorSetLayout(device, &layout_info, nullptr, &descriptor_set_layout) != VK_SUCCESS)
		throw std::runtime_error("failed to create descriptor set layout!");

	return descriptor_set_layout;
}
