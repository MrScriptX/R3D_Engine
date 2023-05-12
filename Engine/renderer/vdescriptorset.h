#ifndef VRED_RENDERER_VDESCRIPTORSET_H
#define VRED_RENDERER_VDESCRIPTORSET_H

#include <vulkan/vulkan.h>
#include <vector>

namespace vred::renderer
{
	VkDescriptorSetLayoutBinding create_descriptor_set_layout_binding(VkDescriptorType type, VkShaderStageFlags flags, uint32_t count, uint32_t binding = 0);
	VkDescriptorSetLayout create_descriptor_set_layout(const VkDevice& device, const std::vector<VkDescriptorSetLayoutBinding>& bindings);
}

#endif // !VRED_RENDERER_VDESCRIPTORSET_H
