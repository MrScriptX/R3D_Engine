#include "helper.h"

#include <stdexcept>

uint32_t vred::renderer::find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& physical_device)
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
