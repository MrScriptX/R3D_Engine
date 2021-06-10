#include "../Includes/renderer/VulkanSwapchain.h"

VulkanSwapchain::VulkanSwapchain(Graphics& m_graphic, uint32_t width, uint32_t height) : m_graphic(m_graphic), WIDTH(width), HEIGHT(height)
{
	createSwapchain();
}

VulkanSwapchain::~VulkanSwapchain()
{
}

void VulkanSwapchain::createSwapchain()
{
	SwapchainDetails swapchain_support = querySwapChainSupport(m_graphic.physical_device);
	VkSurfaceFormatKHR surface_format = chooseSwapSurfaceFormat(swapchain_support.formats);
	VkPresentModeKHR present_mode = chooseSwapPresentMode(swapchain_support.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapchain_support.capabilities);

	WIDTH = extent.width;
	HEIGHT = extent.height;

	uint32_t image_count = swapchain_support.capabilities.minImageCount + 1;
	if (swapchain_support.capabilities.maxImageCount > 0 && image_count > swapchain_support.capabilities.maxImageCount)
	{
		image_count = swapchain_support.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR swapchain_info = {};
	swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_info.pNext = nullptr;
	swapchain_info.flags = 0;

	swapchain_info.surface = m_graphic.surface;
	swapchain_info.minImageCount = image_count;
	swapchain_info.imageFormat = surface_format.format;
	swapchain_info.imageColorSpace = surface_format.colorSpace;
	swapchain_info.imageExtent = extent;
	swapchain_info.imageArrayLayers = 1;
	swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	uint32_t queue_family_indices[] = { static_cast<uint32_t>(m_graphic.queue_indices.graphic_family), static_cast<uint32_t>(m_graphic.queue_indices.present_family) };
	if (m_graphic.queue_indices.graphic_family != m_graphic.queue_indices.present_family)
	{
		swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchain_info.queueFamilyIndexCount = 2;
		swapchain_info.pQueueFamilyIndices = queue_family_indices;
	}
	else
	{
		swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchain_info.queueFamilyIndexCount = 0;
		swapchain_info.pQueueFamilyIndices = nullptr;
	}

	swapchain_info.preTransform = swapchain_support.capabilities.currentTransform;
	swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_info.presentMode = present_mode;
	swapchain_info.clipped = VK_TRUE;
	swapchain_info.oldSwapchain = VK_NULL_HANDLE;

	m_graphic.swapchain_details.format = surface_format.format;
	m_graphic.swapchain_details.extent = extent;

	if (vkCreateSwapchainKHR(m_graphic.device, &swapchain_info, nullptr, &m_graphic.swapchain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swapchain!");
	}

	vkGetSwapchainImagesKHR(m_graphic.device, m_graphic.swapchain, &image_count, nullptr);

	m_graphic.swapchain_images.resize(image_count);
	if (vkGetSwapchainImagesKHR(m_graphic.device, m_graphic.swapchain, &image_count, m_graphic.swapchain_images.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to retrieve swapchain images!");
	}

	createImagesView();
}

const uint32_t& VulkanSwapchain::GetHeigth()
{
	return HEIGHT;
}

void VulkanSwapchain::createImagesView()
{
	m_graphic.images_view.resize(m_graphic.swapchain_images.size());

	for (uint32_t i = 0; i < m_graphic.swapchain_images.size(); i++)
	{
		VkImageViewCreateInfo view_info = {};
		view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.image = m_graphic.swapchain_images[i];
		view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		view_info.format = m_graphic.swapchain_details.format;
		view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		view_info.subresourceRange.baseMipLevel = 0;
		view_info.subresourceRange.levelCount = 1;
		view_info.subresourceRange.baseArrayLayer = 0;
		view_info.subresourceRange.layerCount = 1;

		if (vkCreateImageView(m_graphic.device, &view_info, nullptr, &m_graphic.images_view[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create texture image view!");
		}
	}
}

VkSurfaceFormatKHR VulkanSwapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR VulkanSwapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			bestMode = availablePresentMode;
		}
	}

	return bestMode;
}

VkExtent2D VulkanSwapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		VkExtent2D actualExtent = { WIDTH, HEIGHT };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

SwapchainDetails VulkanSwapchain::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapchainDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_graphic.surface, &details.capabilities);

	uint32_t format_count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_graphic.surface, &format_count, nullptr);

	if (format_count != 0)
	{
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_graphic.surface, &format_count, details.formats.data());
	}

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_graphic.surface, &present_mode_count, nullptr);

	if (present_mode_count != 0)
	{
		details.presentModes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_graphic.surface, &present_mode_count, details.presentModes.data());
	}

	return details;
}
