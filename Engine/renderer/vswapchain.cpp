#include "vswapchain.h"

#include <stdexcept>

VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats);
VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR>& available_modes);
VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR& capabilities, const VkExtent2D& current_extent);

vred::renderer::iswapchain vred::renderer::create_swapchain(const ihardware& hw, const VkExtent2D& current_extent)
{
	const swapchain_details details = query_swapchain_support(hw.physical_device, hw);
	const VkPresentModeKHR present_mode = choose_present_mode(details.present_modes);
	const VkSurfaceFormatKHR surface_format = choose_surface_format(details.formats);
	const VkExtent2D extent = choose_extent(details.capabilities, current_extent);

	VkSwapchainCreateInfoKHR swapchain_info = {};
	swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_info.pNext = nullptr;
	swapchain_info.flags = 0;
	swapchain_info.surface = hw.surface;

	uint32_t image_count = details.capabilities.minImageCount + 1;
	if (details.capabilities.maxImageCount > 0 && image_count > details.capabilities.maxImageCount)
	{
		image_count = details.capabilities.maxImageCount;
	}

	swapchain_info.minImageCount = image_count;
	swapchain_info.imageFormat = surface_format.format;
	swapchain_info.imageColorSpace = surface_format.colorSpace;
	swapchain_info.imageExtent = extent;
	swapchain_info.imageArrayLayers = 1;
	swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const uint32_t queue_family_indices[] = { static_cast<uint32_t>(hw.queue_indices.graphic_family), static_cast<uint32_t>(hw.queue_indices.present_family) };
	if (hw.queue_indices.graphic_family != hw.queue_indices.present_family)
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

	swapchain_info.preTransform = details.capabilities.currentTransform;
	swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_info.presentMode = present_mode;
	swapchain_info.clipped = VK_TRUE;
	swapchain_info.oldSwapchain = VK_NULL_HANDLE;

	iswapchain swapchain;
	if (vkCreateSwapchainKHR(hw.device, &swapchain_info, nullptr, &swapchain.handle) != VK_SUCCESS)
		throw std::runtime_error("failed to create swapchain!");

	swapchain.format = surface_format.format;
	swapchain.extent = extent;

	return swapchain;
}

std::vector<VkImage> vred::renderer::create_swapchain_images(const VkDevice& device, const VkSwapchainKHR& swapchain)
{
	uint32_t image_count = 0;
	vkGetSwapchainImagesKHR(device, swapchain, &image_count, nullptr);

	std::vector<VkImage> images(image_count);
	if (vkGetSwapchainImagesKHR(device, swapchain, &image_count, images.data()) != VK_SUCCESS)
		throw std::runtime_error("Failed to retrieve swapchain images!");

	return images;
}

std::vector<VkImageView> vred::renderer::create_swapchain_views(const VkDevice& device, const iswapchain& swapchain)
{
	std::vector<VkImageView> image_views(swapchain.images.size());
	for (size_t i = 0; i < image_views.size(); ++i)
	{
		VkImageViewCreateInfo view_info = {};
		view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.image = swapchain.images[i];
		view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		view_info.format = swapchain.format;
		view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		view_info.subresourceRange.baseMipLevel = 0;
		view_info.subresourceRange.levelCount = 1;
		view_info.subresourceRange.baseArrayLayer = 0;
		view_info.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device, &view_info, nullptr, &image_views[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create texture image view!");
	}

	return image_views;
}

vred::renderer::swapchain_details vred::renderer::query_swapchain_support(const VkPhysicalDevice& device, const vred::renderer::ihardware& hw)
{
	swapchain_details details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, hw.surface, &details.capabilities);

	uint32_t format_count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, hw.surface, &format_count, nullptr);

	if (format_count != 0)
	{
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, hw.surface, &format_count, details.formats.data());
	}

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, hw.surface, &present_mode_count, nullptr);

	if (present_mode_count != 0)
	{
		details.present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, hw.surface, &present_mode_count, details.present_modes.data());
	}

	return details;
}

VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats)
{
	if (available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED)
	{
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& format : available_formats)
	{
		if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return format;
		}
	}

	return available_formats[0];
}

VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR>& available_modes)
{
	VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& mode : available_modes)
	{
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return mode;
		}
		else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			best_mode = mode;
		}
	}

	return best_mode;
}

VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR& capabilities, const VkExtent2D& current_extent)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		VkExtent2D extent = current_extent;

		extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, current_extent.width));
		extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, current_extent.height));

		return extent;
	}
}
