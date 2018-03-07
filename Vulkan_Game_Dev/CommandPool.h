#pragma once

#include "VkHeader.h"
#include "Queue.h"
#include <stdexcept>

class CommandPool
{
public:
	CommandPool();
	~CommandPool();

	void createCommandPool(VkDevice& device, VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);
	void clean(VkDevice& device);

	VkCommandPool& get();

private:

	VkCommandPool m_commandPool;
};

