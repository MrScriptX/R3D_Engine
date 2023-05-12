#ifndef VRED_RENDERER_VFRAMEBUFFER_H
#define VRED_RENDERER_VFRAMEBUFFER_H

#include <vulkan/vulkan.h>
#include <vector>

#include "iswapchain.h"

namespace vred::renderer
{
	VkFramebuffer create_framebuffer(const VkDevice& device, const VkRenderPass& renderpass, const std::vector<VkImageView>& image_views, VkExtent2D extent);
}

#endif // !VRED_RENDERER_VFRAMEBUFFER_H
