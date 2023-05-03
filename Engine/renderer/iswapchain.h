#ifndef VRED_RENDERER_ISWAPCHAIN_H
#define VRED_RENDERER_ISWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <vector>

namespace vred::renderer
{
	struct iswapchain
	{
		VkSwapchainKHR handle;
		VkFormat format;
		VkExtent2D extent;

		std::vector<VkImage> images;
		std::vector<VkImageView> images_view;

		VkRenderPass main_render_pass;
		VkRenderPass ui_render_pass;
	};
}

#endif // !VRED_RENDERER_ISWAPCHAIN_H
