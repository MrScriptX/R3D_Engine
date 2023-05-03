#ifndef VRED_RENDERER_VRENDERPASS_H
#define VRED_RENDERER_VRENDERPASS_H

#include <vulkan/vulkan.h>

namespace vred::renderer
{
	VkRenderPass create_renderpass(const VkFormat& depth_format, const VkFormat& color_format, const VkDevice& device);
}

#endif // !VRED_RENDERER_VRENDERPASS_H
