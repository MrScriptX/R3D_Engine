#ifndef R3DENGINE_VULKAN_DEVICE_H_
#define R3DENGINE_VULKAN_DEVICE_H_

#include <iostream>
#include <set>

#include "../graphics/Graphics.h"

class VulkanDevice
{
  public:
	VulkanDevice(Graphics& m_graphic);
	~VulkanDevice() = default;

  private:
	void choosePhysicalDevice();
	void createLogicalDevice();

	bool checkDeviceSuitability(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	QueueFamilyIndices findQueueFamily(VkPhysicalDevice device);
	SwapchainDetails querySwapChainSupport(VkPhysicalDevice device);

	Graphics& m_graphic;
};

#endif // !R3DENGINE_VULKAN_DEVICE_H_
