#include "Swapchain.h"

#include <iostream>

Swapchain::Swapchain(Renderer::Vulkan& vulkan) : m_vulkan(vulkan)
{
	VkSurfaceCapabilitiesKHR surface_capabilities;
	if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_vulkan.physicalDevice, m_vulkan.surface, &surface_capabilities) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to acquire surface capabilities!");
	}

	uint32_t format_count = 0;
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(m_vulkan.physicalDevice, m_vulkan.surface, &format_count, nullptr) != VK_SUCCESS)
	{
		throw std::runtime_error("Error occurred during presentation surface formats enumeration!");
	}

	std::vector<VkSurfaceFormatKHR> surface_formats(format_count);
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(m_vulkan.physicalDevice, m_vulkan.surface, &format_count, &surface_formats[0]) != VK_SUCCESS)
	{
		throw std::runtime_error("Error occurred during presentation surface formats enumeration!");
	}


	uint32_t present_mode_count = 0;
	if (vkGetPhysicalDeviceSurfacePresentModesKHR(m_vulkan.physicalDevice, m_vulkan.surface, &present_mode_count, nullptr) != VK_SUCCESS || present_mode_count == 0)
	{
		throw std::runtime_error("Error occurred during presentation surface present modes enumeration!");
	}

	std::vector<VkPresentModeKHR> present_mode(present_mode_count);
	if (vkGetPhysicalDeviceSurfacePresentModesKHR(m_vulkan.physicalDevice, m_vulkan.surface, &present_mode_count, &present_mode[0]))
	{
		throw std::runtime_error("Error occurred during presentation surface present modes enumeration!");
	}


}


Swapchain::~Swapchain()
{
}

uint32_t Swapchain::getSwapchainImageCount(VkSurfaceCapabilitiesKHR & surface_capabilities)
{
	uint32_t image_count = surface_capabilities.minImageCount + 1;
	if ((surface_capabilities.maxImageCount > 0) && (image_count > surface_capabilities.maxImageCount))
	{
		image_count = surface_capabilities.maxImageCount;
	}
	return image_count;
}

VkSurfaceFormatKHR Swapchain::getSwapchainSurfaceFormat(std::vector<VkSurfaceFormatKHR>& surface_format)
{
	if ((surface_format.size() == 1) && (surface_format[0].format == VK_FORMAT_UNDEFINED))
	{
		return{ VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
	}

	for (VkSurfaceFormatKHR &surface_format : surface_format)
	{
		if (surface_format.format == VK_FORMAT_R8G8B8A8_UNORM) 
		{
			return surface_format;
		}
	}

	return surface_format[0];
}

VkExtent2D Swapchain::getSwapchainExtent(VkSurfaceCapabilitiesKHR & surface_capabilities)
{
	if (surface_capabilities.currentExtent.width == -1)
	{
		VkExtent2D swap_chain_extent = { 640, 480 };
		if (swap_chain_extent.width < surface_capabilities.minImageExtent.width)
		{
			swap_chain_extent.width = surface_capabilities.minImageExtent.width;
		}

		if (swap_chain_extent.height < surface_capabilities.minImageExtent.height) 
		{
			swap_chain_extent.height = surface_capabilities.minImageExtent.height;
		}

		if (swap_chain_extent.width > surface_capabilities.maxImageExtent.width)
		{
			swap_chain_extent.width = surface_capabilities.maxImageExtent.width;
		}

		if (swap_chain_extent.height > surface_capabilities.maxImageExtent.height) 
		{
			swap_chain_extent.height = surface_capabilities.maxImageExtent.height;
		}

		return swap_chain_extent;
	}

	return surface_capabilities.currentExtent;
}

VkImageUsageFlags Swapchain::getSwapchainUsageFlags(VkSurfaceCapabilitiesKHR & surface_capabilities)
{
	if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) 
	{
		return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}

	std::cout << "VK_IMAGE_USAGE_TRANSFER_DST image usage is not supported by the swap chain!" << std::endl
		<< "Supported swap chain's image usages include:" << std::endl
		<< (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT ? "    VK_IMAGE_USAGE_TRANSFER_SRC\n" : "")
		<< (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT ? "    VK_IMAGE_USAGE_TRANSFER_DST\n" : "")
		<< (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT ? "    VK_IMAGE_USAGE_SAMPLED\n" : "")
		<< (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT ? "    VK_IMAGE_USAGE_STORAGE\n" : "")
		<< (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_COLOR_ATTACHMENT\n" : "")
		<< (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT\n" : "")
		<< (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT\n" : "")
		<< (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_INPUT_ATTACHMENT" : "")
		<< std::endl;

	return static_cast<VkImageUsageFlags>(-1);
}

VkSurfaceTransformFlagsKHR Swapchain::getSwapchainTransform(VkSurfaceCapabilitiesKHR & surface_capabilities)
{
	if (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		return surface_capabilities.currentTransform;
	}
}

VkPresentModeKHR Swapchain::getSwapchainPresentMode(std::vector<VkPresentModeKHR> & present_modes)
{
	for (VkPresentModeKHR &present_mode : present_modes)
	{
		if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return present_mode;
		}
	}

	for (VkPresentModeKHR &present_mode : present_modes)
	{
		if (present_mode == VK_PRESENT_MODE_FIFO_KHR)
		{
			return present_mode;
		}
	}
	std::cout << "FIFO present mode is not supported by the swap chain!" << std::endl;
	return static_cast<VkPresentModeKHR>(-1);
}
