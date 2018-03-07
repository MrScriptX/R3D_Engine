#include "CommandBuffer.h"



CommandBuffer::CommandBuffer()
{
}


CommandBuffer::~CommandBuffer()
{
}

void CommandBuffer::createCommandBuffer(VkDevice& device, VkCommandPool& commandPool, VkRenderPass& renderPass, VkPipeline& pipeline, VkBuffer& vertexBuffer, VkBuffer& indexBuffer, VkPipelineLayout& pipelineLayout, VkDescriptorSet& descriptorSet, VkExtent2D& swapChainExtent, std::vector<VkFramebuffer>& swapChainBuffer)
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

	for (size_t i = 0; i < m_commandBuffer.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional

		vkBeginCommandBuffer(m_commandBuffer[i], &beginInfo);

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainBuffer[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(m_commandBuffer[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(m_commandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		VkBuffer vertexBuffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_commandBuffer[i], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(m_commandBuffer[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindDescriptorSets(m_commandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

		vkCmdDrawIndexed(m_commandBuffer[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
		vkCmdEndRenderPass(m_commandBuffer[i]);

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

std::vector<VkCommandBuffer>& CommandBuffer::getCommandBuffer()
{
	return m_commandBuffer;
}
