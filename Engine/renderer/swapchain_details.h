#ifndef VRED_RENDERER_SWAPCHAIN_DETAILS_H
#define VRED_RENDERER_SWAPCHAIN_DETAILS_H

#include <vulkan/vulkan.h>
#include <vector>

namespace vred::renderer
{
	struct swapchain_details
	{
	    VkSurfaceCapabilitiesKHR capabilities;
	    std::vector<VkSurfaceFormatKHR> formats;
	    std::vector<VkPresentModeKHR> present_modes;
	};
}

#endif // !VRED_RENDERER_SWAPCHAIN_DETAILS_H
