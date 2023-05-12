#include "vimage.h"

#include <stdexcept>

uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& physical_device);

VkImage vred::renderer::create_image(const VkDevice& device, VkExtent3D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage)
{
	VkImageCreateInfo image_info = {};
	image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.imageType = VK_IMAGE_TYPE_2D;
	image_info.extent = extent;
	image_info.mipLevels = 1;
	image_info.arrayLayers = 1;

	image_info.format = format;
	image_info.tiling = tiling;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_info.usage = usage;
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	image_info.samples = VK_SAMPLE_COUNT_1_BIT;
	image_info.flags = 0;

	VkImage image;
	if (vkCreateImage(device, &image_info, nullptr, &image) != VK_SUCCESS)
		throw std::runtime_error("failed to create image !");

	return image;
}

VkDeviceMemory vred::renderer::create_image_memory(const VkDevice& device, const VkPhysicalDevice& physical_device, const VkImage& image, VkMemoryPropertyFlags properties)
{
	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(device, image, &requirements);

	VkMemoryAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = requirements.size;
	alloc_info.memoryTypeIndex = find_memory_type(requirements.memoryTypeBits, properties, physical_device);

	VkDeviceMemory memory = VK_NULL_HANDLE;
	if (vkAllocateMemory(device, &alloc_info, nullptr, &memory) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate image memory !");

	vkBindImageMemory(device, image, memory, 0);

	return memory;
}

VkImageView vred::renderer::create_image_view(const VkDevice& device, VkImage image, VkFormat format, VkImageAspectFlags flags)
{
	VkImageViewCreateInfo view_info = {};
	view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view_info.image = image;
	view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	view_info.format = format;
	view_info.subresourceRange.aspectMask = flags;
	view_info.subresourceRange.baseMipLevel = 0;
	view_info.subresourceRange.levelCount = 1;
	view_info.subresourceRange.baseArrayLayer = 0;
	view_info.subresourceRange.layerCount = 1;

	VkImageView image_view;
	if (vkCreateImageView(device, &view_info, nullptr, &image_view) != VK_SUCCESS)
		throw std::runtime_error("failed to create texture image view!");

	return image_view;
}

uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& physical_device)
{
	VkPhysicalDeviceMemoryProperties mem_properties;
	vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_properties);

	for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
	{
		if (type_filter & (1 << i) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}
