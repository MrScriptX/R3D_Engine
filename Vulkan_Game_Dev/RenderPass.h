#pragma once

#include "VkHeader.h"
#include <stdexcept>

class RenderPass
{
public:
	RenderPass(VkDevice& device, VkFormat& swapChainImageFormat);
	~RenderPass();

	VkRenderPass& get();

private:

	VkDevice & m_device;
	VkRenderPass m_renderPass;
};

