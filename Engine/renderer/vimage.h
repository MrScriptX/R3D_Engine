#ifndef VRED_RENDERER_VIMAGE_H
#define VRED_RENDERER_VIMAGE_H

#include <vulkan/vulkan.h>

namespace vred::renderer
{
	VkImage create_image(const VkDevice& device, VkExtent3D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage);
	VkDeviceMemory create_image_memory(const VkDevice& device, const VkPhysicalDevice& physical_device, const VkImage& image, VkMemoryPropertyFlags properties);
	VkImageView create_image_view(const VkDevice& device, VkImage image, VkFormat format, VkImageAspectFlags flags);
}

#endif // !VRED_RENDERER_VIMAGE_H
