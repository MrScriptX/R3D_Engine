#pragma once

#include "VkHeader.h"
#include "Queue.h"
#include <stdexcept>

class CommandPool
{
public:
	CommandPool(VkDevice const& device, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);
	~CommandPool();

	VkCommandPool& get();

private:

	VkDevice const& m_device;
	VkCommandPool m_commandPool;
};

