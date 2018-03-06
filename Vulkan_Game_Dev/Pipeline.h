#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#include "vertex.h"
#include <vector>
#include <fstream>

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	void clean(VkDevice& device);

	void createPipeline(VkDevice& device, VkExtent2D& swapChainExtent);
	void createRenderPass(VkDevice& device, VkFormat& swapChainImageFormat);
	VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice& device);

	std::vector<char> loadFromFile(const std::string& filename);
	
	//new
	VkRenderPass* getRenderPass();
	VkPipeline* getPipeline();

private:
	VkPipeline m_graphicsPipeline;
	VkRenderPass m_renderPass;
	VkPipelineLayout m_pipelineLayout;
};

