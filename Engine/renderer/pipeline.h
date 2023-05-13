#ifndef VRED_RENDERER_PIPELINE_H
#define VRED_RENDERER_PIPELINE_H

#include <vulkan/vulkan.h>
#include <string>

namespace vred::renderer
{
	VkPipelineShaderStageCreateInfo create_shader_stage_info(VkShaderStageFlagBits stage, VkShaderModule shader, const std::string& entry_point = "main");
}

#endif // !VRED_RENDERER_PIPELINE_H
