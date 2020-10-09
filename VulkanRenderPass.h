#ifndef _VULKAN_RENDERPASS_H
#define _VULKAN_RENDERPASS_H

#include <array>

#include "Graphics.h"
#include <stdexcept>

class VulkanRenderPass
{
public:
	VulkanRenderPass(Graphics & m_graphic, VkFormat depthFormat);
	~VulkanRenderPass();

	void createRenderPass(VkFormat depthFormat);

	void beginRenderPass(VkCommandBuffer& commanderBuffer, VkRenderPassBeginInfo& renderPass_info);
	void endRenderPass(VkCommandBuffer& commandBuffer);

private:

	Graphics & m_graphic;
};

#endif // !_VULKAN_RENDERPASS_H