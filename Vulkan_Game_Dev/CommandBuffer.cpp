#include "CommandBuffer.h"


#include <array>


CommandBuffer::CommandBuffer()
{
}


CommandBuffer::~CommandBuffer()
{
}

void CommandBuffer::allocateCommandBuffer(VkDevice& device, VkCommandPool& commandPool, std::vector<VkFramebuffer>& swapChainBuffer)
{
	m_commandBuffer.resize(swapChainBuffer.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)m_commandBuffer.size();

	if (vkAllocateCommandBuffers(device, &allocInfo, m_commandBuffer.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate commad buffer!");
	}
}

void CommandBuffer::beginCommandBuffer(VkRenderPass& renderPass, VkPipeline& pipeline, VkBuffer& vertexBuffer, VkBuffer& indexBuffer, VkPipelineLayout& pipelineLayout, VkDescriptorSet& descriptorSet, VkExtent2D& swapChainExtent, std::vector<VkFramebuffer>& swapChainBuffer, std::vector<uint32_t>& indices)
{
	for (size_t i = 0; i < m_commandBuffer.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional

		vkBeginCommandBuffer(m_commandBuffer[i], &beginInfo);
	}
}

void CommandBuffer::endCommandBuffer()
{
	for (size_t i = 0; i < m_commandBuffer.size(); i++)
	{
		if (vkEndCommandBuffer(m_commandBuffer[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void CommandBuffer::clean(VkDevice & device, VkCommandPool & commandPool)
{
	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(m_commandBuffer.size()), m_commandBuffer.data());
}

VkCommandBuffer CommandBuffer::beginSingleTimeCommands(VkDevice& device, VkCommandPool& commandPool)
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void CommandBuffer::endSingleTimeCommands(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkCommandBuffer & commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);
	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

std::vector<VkCommandBuffer>& CommandBuffer::getCommandBuffer()
{
	return m_commandBuffer;
}
