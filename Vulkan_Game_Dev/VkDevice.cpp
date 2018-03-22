#include "VkDevice.h"

#include <set>

Device::Device(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface)
{
	constant constantVar;
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentationFamily };

	float queuePriority = 1.0f;
	for (int queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}


	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.shaderClipDistance = VK_TRUE;
	deviceFeatures.shaderCullDistance = VK_TRUE;
	deviceFeatures.geometryShader = VK_TRUE;
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t> (constantVar.deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = constantVar.deviceExtensions.data();

	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(constantVar.validationLayers.size());
		createInfo.ppEnabledLayerNames = constantVar.validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(m_device, indices.graphicsFamily, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, indices.presentationFamily, 0, &m_presentQueue);
}


Device::~Device()
{
	vkDestroyDevice(m_device, nullptr);
}

VkDevice& Device::getDevice()
{
	return m_device;
}

VkQueue& Device::getGraphicsQueue()
{
	return m_graphicsQueue;
}

VkQueue& Device::getPresentQueue()
{
	return m_presentQueue;
}
