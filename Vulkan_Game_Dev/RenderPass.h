#pragma once

#include "VkHeader.h"
#include <vector>
#include <stdexcept>

class RenderPass
{
public:
	RenderPass(VkDevice& device, VkFormat& swapChainImageFormat, VkFormat depthFormat);
	~RenderPass();

	void beginRenderPass(std::vector<VkCommandBuffer> commandBuffer, VkExtent2D& swapChainExtent, std::vector<VkFramebuffer>& swapChainBuffer);

	VkRenderPass& get();

private:

	VkDevice & m_device;
	VkRenderPass m_renderPass;
};

