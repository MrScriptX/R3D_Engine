#ifndef VRED_RENDERER_VPIPELINE_H
#define VRED_RENDERER_VPIPELINE_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

#include "iswapchain.h"
#include "pipeline.h"

namespace vred::renderer
{
	struct shader_stages
	{
		std::string vertex;
		std::string fragment;

		VkPolygonMode polygon_mode = VkPolygonMode::VK_POLYGON_MODE_FILL;
	};

	VkPipeline create_pipeline(const shader_stages& stages, const VkPipelineLayout& layout, const VkDevice& device, const iswapchain& swapchain);
	VkPipelineLayout create_pipeline_layout(const VkDevice& device, const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts);
	void destroy_pipeline(const vred::renderer::ipipeline& pipeline, const VkDevice& device);

	VkPipelineShaderStageCreateInfo create_shader_stage_info(VkShaderStageFlagBits stage, VkShaderModule shader);
	VkPipelineVertexInputStateCreateInfo create_vertex_input_state_info(const VkVertexInputBindingDescription& binding, const std::vector<VkVertexInputAttributeDescription>& attributes);
	VkPipelineInputAssemblyStateCreateInfo create_input_assembly_state_info();
	VkPipelineViewportStateCreateInfo create_viewport_state_info(const VkViewport& viewport, const VkRect2D& scissor);
	VkPipelineRasterizationStateCreateInfo create_rasterization_state_info(VkPolygonMode polygone_mode);
	VkPipelineMultisampleStateCreateInfo create_multisample_state_info();
	VkPipelineDepthStencilStateCreateInfo create_depth_stencil_state_info();
	VkPipelineColorBlendStateCreateInfo create_color_blend_state_info(const VkPipelineColorBlendAttachmentState& color_blend_attachment);
}

#endif // !VRED_RENDERER_VPIPELINE_H
