#include "FrameBuffer.h"



FrameBuffer::FrameBuffer(VkDevice& device, VkRenderPass& renderPass, std::vector<VkImageView>& m_swapChainImageViews, VkExtent2D& m_swapChainExtent) : m_device(device)
{
	m_swapChainFrameBuffer.resize(m_swapChainImageViews.size());//set vector size to hold all images

	for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
	{
		VkImageView attachement[] = { m_swapChainImageViews[i] };

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachement;
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
