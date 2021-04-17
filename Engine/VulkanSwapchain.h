#ifndef _VULKAN_SWAPCHAIN_H
#define _VULKAN_SWAPCHAIN_H

#include <algorithm>
#include <stdexcept>

#include "Config.h"
#include "Graphics.h"

class VulkanSwapchain
{
public:
	VulkanSwapchain(Graphics & m_graphic, uint32_t width, uint32_t height);
	~VulkanSwapchain();

	void createSwapchain();
private:
	void createImagesView();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	SwapchainDetails querySwapChainSupport(VkPhysicalDevice device);

	uint32_t WIDTH;
	uint32_t HEIGHT;

	Graphics & m_graphic;
};

#endif // !_VULKAN_SWAPCHAIN_H