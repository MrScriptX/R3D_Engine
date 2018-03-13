#pragma once

#include "VkHeader.h"
#include <stdexcept>
#include "vertex.h"

class CommandBuffer
{
public:
	CommandBuffer();
	~CommandBuffer();

	void allocateCommandBuffer(VkDevice& device, VkCommandPool& commandPool, std::vector<VkFramebuffer>& swapChainBuffer);
	void beginCommandBuffer(VkRenderPass& renderPass, VkPipeline& pipeline, VkBuffer& vertexBuffer, VkBuffer& indexBuffer, VkPipelineLayout& pipelineLayout, VkDescriptorSet& descriptorSet, VkExtent2D& swapChainExtent, std::vector<VkFramebuffer>& swapChainBuffer, std::vector<uint32_t>& indices);
	void endCommandBuffer();

	void clean(VkDevice& device, VkCommandPool& commandPool);

	static VkCommandBuffer beginSingleTimeCommands(VkDevice& device, VkCommandPool& commandPool);
	static void endSingleTimeCommands(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkCommandBuffer& commandBuffer);

	std::vector<VkCommandBuffer>& getCommandBuffer();
private:

	std::vector<VkCommandBuffer> m_commandBuffer;
};

