#pragma once

#include "Common.h"

class Device
{
public:
	Device(Renderer::Vulkan& vulkan);
	~Device();

private:

	bool checkPhysicalDeviceProperties(VkPhysicalDevice & physical_devices, uint32_t graphics_queue_index, uint32_t present_queue_index);
	bool checkExtensionsAvailability(const char* extensions, std::vector<VkExtensionProperties> available_extensions);

	Renderer::Vulkan& m_vulkan;
};

