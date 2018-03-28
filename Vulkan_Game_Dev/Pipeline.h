#pragma once

#include "VkHeader.h"
#include "vertex.h"
#include "RenderPass.h"
#include <vector>
#include <fstream>

class Pipeline
{
public:
	Pipeline(VkDevice& device, VkExtent2D& swapChainExtent, VkDescriptorSetLayout& descriptorSetLayout, VkRenderPass& renderPass, std::string vertShader, std::string fragShader);
	~Pipeline();

	void createPipeline(VkExtent2D& swapChainExtent, VkDescriptorSetLayout& descriptorSetLayout, VkRenderPass& renderPass);
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void bindPipeline(std::vector<VkCommandBuffer>& commandBuffer);

	std::vector<char> loadFromFile(const std::string& filename);
	
	VkPipeline& getPipeline();
	VkPipelineLayout& getPipelineLayout();

private:
	VkDevice & m_device;

	VkPipeline m_graphicsPipeline;
	VkPipelineLayout m_pipelineLayout;

	std::string m_vertShaderPath;
	std::string m_fragShaderPath;
};

