#pragma once

#include "VkHeader.h"
#include "Buffer.h"
#include <stdexcept>

class UniformBuffer
{
public:
	UniformBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice);
	~UniformBuffer();

	VkDeviceMemory& getBufferMemory();
	VkBuffer& getBuffer();

private:

	VkDevice const& m_device;
	VkBuffer m_uniformBuffer;
	VkDeviceMemory m_uniformBufferMemory;
};

