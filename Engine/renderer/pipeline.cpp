#include "pipeline.h"

VkPipelineShaderStageCreateInfo vred::renderer::create_shader_stage_info(VkShaderStageFlagBits stage, VkShaderModule shader, const std::string& entry_point)
{
	VkPipelineShaderStageCreateInfo shader_stage_info = {};
	shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shader_stage_info.module = shader;
	shader_stage_info.pName = entry_point.c_str();

    return shader_stage_info;
}
