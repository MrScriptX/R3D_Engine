#ifndef R3DENGINE_VULKAN_RENDERPASS_H_
#define R3DENGINE_VULKAN_RENDERPASS_H_

#include <array>
#include <stdexcept>

#include "../graphics/Graphics.h"

class VulkanRenderPass
{
  public:
	VulkanRenderPass(Graphics& m_graphic, VkFormat depthFormat);
	~VulkanRenderPass();

	void createRenderPass(VkFormat depthFormat);

	void beginRenderPass(VkCommandBuffer& commanderBuffer, VkRenderPassBeginInfo& renderPass_info);
	void endRenderPass(VkCommandBuffer& commandBuffer);

  private:
	Graphics& m_graphic;
};

#endif // !R3DENGINE_VULKAN_RENDERPASS_H_
