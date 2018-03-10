#include "FrameBuffer.h"

#include <array>


FrameBuffer::FrameBuffer(VkDevice& device, VkRenderPass& renderPass, std::vector<VkImageView>& m_swapChainImageViews, VkExtent2D& m_swapChainExtent, VkImageView& depthImageView) : m_device(device)
{
	m_swapChainFrameBuffer.resize(m_swapChainImageViews.size());//set vector size to hold all images

	for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
	{
		std::array<VkImageView, 2> attachment = { m_swapChainImageViews[i], depthImageView };
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachment.size());
		framebufferInfo.pAttachments = attachment.data();
		framebufferInfo.width = m_swapChainExtent.width;
		framebufferInfo.height = m_swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_swapChainFrameBuffer[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create frame buffer!");
		}
	}
}


FrameBuffer::~FrameBuffer()
{
	for (auto frameBuffer : m_swapChainFrameBuffer)
	{
		vkDestroyFramebuffer(m_device, frameBuffer, nullptr);
	}

	std::clog << "FrameBuffers destroyed\n";
}

std::vector<VkFramebuffer>& FrameBuffer::getFrameBuffer()
{
	return m_swapChainFrameBuffer;
}
