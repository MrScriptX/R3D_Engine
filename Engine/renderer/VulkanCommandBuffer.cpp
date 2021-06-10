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
