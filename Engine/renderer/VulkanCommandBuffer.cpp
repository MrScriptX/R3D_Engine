#include "../Includes/renderer/VulkanCommandBuffer.h"



VulkanCommandBuffer::VulkanCommandBuffer(Graphics& m_graphic) : m_graphic(m_graphic)
{
}


VulkanCommandBuffer::~VulkanCommandBuffer()
{
}

void VulkanCommandBuffer::allocateCommandBuffer()
{
	m_graphic.command_buffers.resize(m_graphic.framebuffers.size());

	VkCommandBufferAllocateInfo alloc_buffers_info = {};
	alloc_buffers_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_buffers_info.commandPool = m_graphic.command_pool;
	alloc_buffers_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_buffers_info.commandBufferCount = static_cast<uint32_t>(m_graphic.command_buffers.size());

	if (vkAllocateCommandBuffers(m_graphic.device, &alloc_buffers_info, m_graphic.command_buffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

/*void VulkanCommandBuffer::recordCommandBuffer()
{
	std::array<VkClearValue, 2> clear_values = {};
	clear_values[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clear_values[1].depthStencil = { 1.0f, 0 };

	for (size_t i = 0; i < m_graphic.command_buffers.size(); i++)
	{
		VkCommandBufferBeginInfo begin_buffer_info = {};
		begin_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_buffer_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		begin_buffer_info.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(m_graphic.command_buffers[i], &begin_buffer_info) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_graphic.render_pass;
		renderPassInfo.framebuffer = m_graphic.framebuffers[i];

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_graphic.swapchain_details.extent;

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clear_values.size());
		renderPassInfo.pClearValues = clear_values.data();

		vkCmdBeginRenderPass(m_graphic.command_buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(m_graphic.command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getHandle());

		VkBuffer vertexBuffers[] = { m_graphic.vertex_buffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_graphic.command_buffers[i], 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(m_graphic.command_buffers[i], m_graphic.index_buffer, 0, VK_INDEX_TYPE_UINT16);

		vkCmdBindDescriptorSets(m_graphic.command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getLayout(), 0, 1, &m_graphic.descriptor_set, 0, nullptr);

		vkCmdDrawIndexed(m_graphic.command_buffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

		vkCmdEndRenderPass(m_graphic.command_buffers[i]);

		if (vkEndCommandBuffer(m_graphic.command_buffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}*/
