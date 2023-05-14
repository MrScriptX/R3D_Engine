#ifndef VRED_RENDERER_VPIPELINE_H
#define VRED_RENDERER_VPIPELINE_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

#include "iswapchain.h"

namespace vred::renderer
{
	struct shader_stages
	{
		std::string vertex;
		std::string fragment;
	};

	VkPipeline create_pipeline(const shader_stages& stages, const VkPipelineLayout& layout, const VkDevice& device, const iswapchain& swapchain, VkPolygonMode polygon_mode = VkPolygonMode::VK_POLYGON_MODE_FILL);
	VkPipelineLayout create_pipeline_layout(const VkDevice& device, const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts);
	
	VkPipelineShaderStageCreateInfo create_shader_stage_info(VkShaderStageFlagBits stage, VkShaderModule shader, const std::string& entry_point = "main");
	VkPipelineVertexInputStateCreateInfo create_vertex_input_state_info();
	VkPipelineInputAssemblyStateCreateInfo create_input_assembly_state_info();
	VkPipelineViewportStateCreateInfo create_viewport_state_info(VkExtent2D extent);
	VkPipelineRasterizationStateCreateInfo create_rasterization_state_info(VkPolygonMode polygone_mode);
	VkPipelineMultisampleStateCreateInfo create_multisample_state_info();
	VkPipelineDepthStencilStateCreateInfo create_depth_stencil_state_info();
	VkPipelineColorBlendStateCreateInfo create_color_blend_state_info();
}

#endif // !VRED_RENDERER_VPIPELINE_H
