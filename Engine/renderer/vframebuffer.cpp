#include "vframebuffer.h"

#include <stdexcept>

VkFramebuffer vred::renderer::create_framebuffer(const VkDevice& device, const VkRenderPass& renderpass, const std::vector<VkImageView>& image_views, VkExtent2D extent)
{
	VkFramebufferCreateInfo framebuffer_info = {};
	framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebuffer_info.renderPass = renderpass;
	framebuffer_info.width = extent.width;
	framebuffer_info.height = extent.height;
	framebuffer_info.layers = 1;
	framebuffer_info.attachmentCount = static_cast<uint32_t>(image_views.size());
	framebuffer_info.pAttachments = image_views.data();

	VkFramebuffer framebuffer;
	if (vkCreateFramebuffer(device, &framebuffer_info, nullptr, &framebuffer) != VK_SUCCESS)
		throw std::runtime_error("failed to create framebuffers!");

	return framebuffer;
}
