#ifndef VRED_RENDERER_VSWAPCHAIN_H
#define VRED_RENDERER_VSWAPCHAIN_H

#include <vulkan/vulkan.h>

#include "ihardware.h"
#include "iswapchain.h"
#include "swapchain_details.h"

namespace vred::renderer
{
	iswapchain create_swapchain(const ihardware& hw, const VkExtent2D& current_extent);
	std::vector<VkImage> create_swapchain_images(const VkDevice& device, const VkSwapchainKHR& swapchain);
	std::vector<VkImageView> create_swapchain_views(const VkDevice& device, const iswapchain& swapchain);
	swapchain_details query_swapchain_support(const VkPhysicalDevice& device, const ihardware& hw);
}

#endif // !VRED_RENDERER_VSWAPCHAIN_H
