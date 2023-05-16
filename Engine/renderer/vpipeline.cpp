#include "vpipeline.h"

#include <stdexcept>

#include "../graphics/Vertex.h"
#include "vshader.h"

VkPipeline vred::renderer::create_pipeline(const shader_stages& stages, const VkPipelineLayout& layout, const VkDevice& device, const iswapchain& swapchain)
{
	// shader stages
	std::vector<VkPipelineShaderStageCreateInfo> shader_stages;

	VkShaderModule vertex_module;
	if (!stages.precompiled)
	{
		std::vector<uint32_t> vertex_shader_code = compile_shader(stages.vertex, shaderc_vertex_shader);
		vertex_module = create_shader_module(vertex_shader_code.data(), vertex_shader_code.size(), device);
	}
	else
	{
		std::vector<char> vertex_shader_code = read_shader_file(stages.vertex);
		vertex_module = create_shader_module(reinterpret_cast<const uint32_t*>(vertex_shader_code.data()), vertex_shader_code.size(), device);
	}

	shader_stages.push_back(create_shader_stage_info(VK_SHADER_STAGE_VERTEX_BIT, vertex_module));


	VkShaderModule fragment_module;
	if (!stages.precompiled)
	{
		std::vector<uint32_t> frag_shader_code = compile_shader(stages.fragment, shaderc_fragment_shader);
		fragment_module = create_shader_module(frag_shader_code.data(), frag_shader_code.size(), device);
	}
	else
	{
		std::vector<char> frag_shader_code = read_shader_file(stages.fragment);
		fragment_module = create_shader_module(reinterpret_cast<const uint32_t*>(frag_shader_code.data()), frag_shader_code.size(), device);
	}
	
	shader_stages.push_back(create_shader_stage_info(VK_SHADER_STAGE_FRAGMENT_BIT, fragment_module));

	VkVertexInputBindingDescription binding_description = Vertex::getBindingDescription();
	std::vector<VkVertexInputAttributeDescription> attribute_descriptions = Vertex::getAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertex_input_state = create_vertex_input_state_info(binding_description, attribute_descriptions);
	
	VkPipelineInputAssemblyStateCreateInfo input_assembly_state = create_input_assembly_state_info();

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapchain.extent.width);
	viewport.height = static_cast<float>(swapchain.extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapchain.extent;

	VkPipelineViewportStateCreateInfo viewport_state = create_viewport_state_info(viewport, scissor);
	
	VkPipelineRasterizationStateCreateInfo rasterizer = create_rasterization_state_info(stages.polygon_mode);
	VkPipelineMultisampleStateCreateInfo multisampler = create_multisample_state_info();
	VkPipelineDepthStencilStateCreateInfo depth_stencil_state = create_depth_stencil_state_info();

	VkPipelineColorBlendAttachmentState color_blend_attachment = {};
	color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo color_blender = create_color_blend_state_info(color_blend_attachment);

	VkGraphicsPipelineCreateInfo pipeline_info = {};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = shader_stages.size();
	pipeline_info.pStages = shader_stages.data();
	pipeline_info.pVertexInputState = &vertex_input_state;
	pipeline_info.pInputAssemblyState = &input_assembly_state;
	pipeline_info.pViewportState = &viewport_state;
	pipeline_info.pRasterizationState = &rasterizer;
	pipeline_info.pMultisampleState = &multisampler;
	pipeline_info.pDepthStencilState = &depth_stencil_state;
	pipeline_info.pColorBlendState = &color_blender;
	pipeline_info.layout = layout;
	pipeline_info.renderPass = swapchain.main_render_pass;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

	VkPipeline pipeline = VK_NULL_HANDLE;
	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline) != VK_SUCCESS)
		throw std::runtime_error("failed to create graphics pipeline!");

	vkDestroyShaderModule(device, vertex_module, nullptr);
	vkDestroyShaderModule(device, fragment_module, nullptr);

	return pipeline;
}

VkPipelineLayout vred::renderer::create_pipeline_layout(const VkDevice& device, const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts)
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = descriptor_set_layouts.size();
	pipelineLayoutInfo.pSetLayouts = descriptor_set_layouts.data();

	VkPipelineLayout pipeline_layout;
	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipeline_layout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout!");

	return pipeline_layout;
}

void vred::renderer::destroy_pipeline(const vred::renderer::ipipeline& pipeline, const VkDevice& device)
{
	vkDeviceWaitIdle(device);

	vkDestroyPipeline(device, pipeline.handle, nullptr);
	vkDestroyPipelineLayout(device, pipeline.layout, nullptr);
}

VkPipelineShaderStageCreateInfo vred::renderer::create_shader_stage_info(VkShaderStageFlagBits stage, VkShaderModule shader)
{
	VkPipelineShaderStageCreateInfo shader_stage_info = {};
	shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stage_info.stage = stage;
	shader_stage_info.module = shader;
	shader_stage_info.pName = "main";

    return shader_stage_info;
}

VkPipelineVertexInputStateCreateInfo vred::renderer::create_vertex_input_state_info(const VkVertexInputBindingDescription& binding, const std::vector<VkVertexInputAttributeDescription>& attributes)
{
	VkPipelineVertexInputStateCreateInfo vertex_input_info = {};
	vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_info.pNext = nullptr;
	vertex_input_info.vertexBindingDescriptionCount = 1;
	vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
	vertex_input_info.pVertexBindingDescriptions = &binding;
	vertex_input_info.pVertexAttributeDescriptions = attributes.data();

	return vertex_input_info;
}

VkPipelineInputAssemblyStateCreateInfo vred::renderer::create_input_assembly_state_info()
{
	VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {};
	input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly_state.primitiveRestartEnable = VK_FALSE;
	
	return input_assembly_state;
}

VkPipelineViewportStateCreateInfo vred::renderer::create_viewport_state_info(const VkViewport& viewport, const VkRect2D& scissor)
{
	VkPipelineViewportStateCreateInfo viewport_state_info = {};
	viewport_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state_info.viewportCount = 1;
	viewport_state_info.pViewports = &viewport;
	viewport_state_info.scissorCount = 1;
	viewport_state_info.pScissors = &scissor;

	return viewport_state_info;
}

VkPipelineRasterizationStateCreateInfo vred::renderer::create_rasterization_state_info(VkPolygonMode polygone_mode)
{
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = polygone_mode;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	return rasterizer;
}

VkPipelineMultisampleStateCreateInfo vred::renderer::create_multisample_state_info()
{
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	return multisampling;
}

VkPipelineDepthStencilStateCreateInfo vred::renderer::create_depth_stencil_state_info()
{
	VkPipelineDepthStencilStateCreateInfo depth_stencil_info = {};
	depth_stencil_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth_stencil_info.depthTestEnable = VK_TRUE;
	depth_stencil_info.depthWriteEnable = VK_TRUE;
	depth_stencil_info.depthCompareOp = VK_COMPARE_OP_LESS;
	depth_stencil_info.depthBoundsTestEnable = VK_FALSE;
	depth_stencil_info.stencilTestEnable = VK_FALSE;

	return depth_stencil_info;
}

VkPipelineColorBlendStateCreateInfo vred::renderer::create_color_blend_state_info(const VkPipelineColorBlendAttachmentState& color_blend_attachment)
{
	VkPipelineColorBlendStateCreateInfo color_blending = {};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.logicOp = VK_LOGIC_OP_COPY;
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;
	color_blending.blendConstants[0] = 0.0f;
	color_blending.blendConstants[1] = 0.0f;
	color_blending.blendConstants[2] = 0.0f;
	color_blending.blendConstants[3] = 0.0f;

	return color_blending;
}
