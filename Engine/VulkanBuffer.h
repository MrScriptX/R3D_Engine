#ifndef _VULKANBUFFER_H
#define _VULKANBUFFER_H

#include <memory>
#include <stdexcept>

#include "Graphics.h"

class VulkanBuffer
{
public:
	VulkanBuffer(Graphics & graphic);
	~VulkanBuffer();

	void createBuffer(VkBuffer & buffer, VkDeviceMemory & bufferMemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

private:

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	Graphics & m_graphic;
};

#endif // !_VULKANBUFFER_H