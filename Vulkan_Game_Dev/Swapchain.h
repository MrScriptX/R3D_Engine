#pragma once

#include "Common.h"

class Swapchain
{
public:
	Swapchain(Renderer::Vulkan& vulkan);
	~Swapchain();

private:

	uint32_t getSwapchainImageCount(VkSurfaceCapabilitiesKHR& surface_capabilities);
	VkSurfaceFormatKHR getSwapchainSurfaceFormat(std::vector<VkSurfaceFormatKHR>& surface_format);
	VkExtent2D getSwapchainExtent(VkSurfaceCapabilitiesKHR& surface_capabilities);
	VkImageUsageFlags getSwapchainUsageFlags(VkSurfaceCapabilitiesKHR& surface_capabilities);
	VkSurfaceTransformFlagsKHR getSwapchainTransform(VkSurfaceCapabilitiesKHR& surface_capabilities);
	VkPresentModeKHR getSwapchainPresentMode(std::vector<VkPresentModeKHR>& present_modes);

	Renderer::Vulkan& m_vulkan;
};

