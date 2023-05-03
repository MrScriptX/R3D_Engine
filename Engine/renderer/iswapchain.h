#ifndef VRED_RENDERER_SWAPCHAIN_H
#define VRED_RENDERER_SWAPCHAIN_H

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
	};
};

#endif // !VRED_RENDERER_SWAPCHAIN_H
