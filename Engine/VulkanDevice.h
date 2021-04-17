#ifndef _VULKAN_DEVICE_H
#define _VULKAN_DEVICE_H

#include <iostream>
#include <set>
#include "Graphics.h"

class VulkanDevice
{
public:
	VulkanDevice(Graphics& m_graphic);
	~VulkanDevice();


private:
	void choosePhysicalDevice();
	void createLogicalDevice();

	bool checkDeviceSuitability(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	QueueFamilyIndices findQueueFamily(VkPhysicalDevice device);
	SwapchainDetails querySwapChainSupport(VkPhysicalDevice device);

	Graphics & m_graphic;
};

#endif _VULKAN_DEVICE_H