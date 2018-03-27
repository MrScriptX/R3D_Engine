#pragma once

#include "VkHeader.h"
#include "Queue.h"
#include "ValidationLayer.h"

class Device
{
public:
	Device(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);
	~Device();
	
	VkDevice& getDevice();
	VkQueue& getGraphicsQueue();
	VkQueue& getPresentQueue();

private:

	VkDevice m_device;
	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;
};

