#pragma once

#include "VkHeader.h"
#include <vector>

class FrameBuffer
{
public:
	FrameBuffer(VkDevice& device, VkRenderPass& renderPass, std::vector<VkImageView>& m_swapChainImageViews, VkExtent2D& m_swapChainExtent, VkImageView& depthImageView);
	~FrameBuffer();

	std::vector<VkFramebuffer>& getFrameBuffer();

private:

	VkDevice & m_device;
	std::vector<VkFramebuffer> m_swapChainFrameBuffer;
};

