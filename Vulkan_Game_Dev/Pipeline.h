#pragma once

#include "VkHeader.h"
#include "vertex.h"
#include <vector>
#include <fstream>

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	void clean(VkDevice& device);

	void createPipeline(VkDevice& device, VkExtent2D& swapChainExtent, VkDescriptorSetLayout& descriptorSetLayout);
	void createRenderPass(VkDevice& device, VkFormat& swapChainImageFormat);
	VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice& device);

	std::vector<char> loadFromFile(const std::string& filename);
	
	//new
	VkRenderPass& getRenderPass();
	VkPipeline& getPipeline();
	VkPipelineLayout& getPipelineLayout();

private:
	VkPipeline m_graphicsPipeline;
	VkRenderPass m_renderPass;
	VkPipelineLayout m_pipelineLayout;
};

