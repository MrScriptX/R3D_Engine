#include "vbuffer.h"

#include <stdexcept>

#include "helper.h"

VkBuffer vred::renderer::create_buffer(const ihardware& hw, VkDeviceSize size, VkBufferUsageFlags usage)
{
	VkBufferCreateInfo buffer_info = {};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.size = size;
	buffer_info.usage = usage;
	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBuffer buffer = VK_NULL_HANDLE;
	if (vkCreateBuffer(hw.device, &buffer_info, nullptr, &buffer) != VK_SUCCESS)
		throw std::runtime_error("Failed to create buffer !");
	
	return buffer;
}

VkDeviceMemory vred::renderer::allocate_buffer(const ihardware& hw, const VkBuffer& buffer, VkMemoryPropertyFlags properties)
{
	VkMemoryRequirements mem_requirements;
	vkGetBufferMemoryRequirements(hw.device, buffer, &mem_requirements);

	VkMemoryAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex = find_memory_type(mem_requirements.memoryTypeBits, properties, hw.physical_device);

	VkDeviceMemory buffer_memory = VK_NULL_HANDLE;
	if (vkAllocateMemory(hw.device, &alloc_info, nullptr, &buffer_memory) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate buffer memory !");

	if (vkBindBufferMemory(hw.device, buffer, buffer_memory, 0) != VK_SUCCESS)
		throw std::runtime_error("Failed to bind buffer !");

	return buffer_memory;
}
