#pragma once

#include "VkHeader.h"
#include <stdexcept>
#include "vertex.h"

class CommandBuffer
{
public:
	CommandBuffer();
	~CommandBuffer();

	void createCommandBuffer(VkDevice& device, VkCommandPool& commandPool, VkRenderPass& renderPass, VkPipeline& pipeline, VkBuffer& vertexBuffer, VkBuffer& indexBuffer, VkPipelineLayout& pipelineLayout, VkDescriptorSet& descriptorSet, VkExtent2D& swapChainExtent, std::vector<VkFramebuffer>& swapChainBuffer);
	void clean(VkDevice& device, VkCommandPool& commandPool);

	std::vector<VkCommandBuffer>& getCommandBuffer();
private:

	std::vector<VkCommandBuffer> m_commandBuffer;
};

