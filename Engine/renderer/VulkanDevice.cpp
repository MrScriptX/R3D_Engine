#include "VulkanDevice.h"



VulkanDevice::VulkanDevice(Graphics& m_graphic) : m_graphic(m_graphic)
{
	choosePhysicalDevice();
	createLogicalDevice();
}


VulkanDevice::~VulkanDevice()
{
}

void VulkanDevice::choosePhysicalDevice()
{
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(m_graphic.instance, &device_count, nullptr);

	if (device_count == 0)
	{
		throw std::runtime_error("Failed to find suitable device");
	}

	std::vector<VkPhysicalDevice> available_device(device_count);
	vkEnumeratePhysicalDevices(m_graphic.instance, &device_count, available_device.data());

	for (uint32_t i = 0; i < available_device.size(); i++)
	{
		if (checkDeviceSuitability(available_device[i]))
		{
			m_graphic.physical_device = available_device[i];
			break;
		}
	}

	if (m_graphic.physical_device == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find a suitable device!");
	}
	else
	{
		VkPhysicalDeviceProperties device_properties;
		vkGetPhysicalDeviceProperties(m_graphic.physical_device, &device_properties);

		std::clog << "Physical Device Name: " << device_properties.deviceName << "\n";
		std::clog << "Physical Device Vendor ID: " << device_properties.vendorID << "\n";
		std::clog << "Physical Device Driver version: " << device_properties.driverVersion << std::endl;
	}
}

void VulkanDevice::createLogicalDevice()
{
	float queuePriority = 1.0f;
	QueueFamilyIndices indices = findQueueFamily(m_graphic.physical_device);
	std::set<int> uniqueQueueFamilies = { indices.graphic_family, indices.present_family };

	std::vector<VkDeviceQueueCreateInfo> queue_infos = {};
	for (int queue_family : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queue_info = {};
		queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info.pNext = nullptr;
		queue_info.flags = 0;

		queue_info.queueFamilyIndex = queue_family;
		queue_info.queueCount = 1;
		queue_info.pQueuePriorities = &queuePriority;

		queue_infos.push_back(queue_info);
	}


	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.fillModeNonSolid = VK_TRUE;


	VkDeviceCreateInfo device_info = {};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pNext = nullptr;
	device_info.flags = 0;

	device_info.queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size());
	device_info.pQueueCreateInfos = queue_infos.data();

	device_info.pEnabledFeatures = &deviceFeatures;
	device_info.enabledExtensionCount = static_cast<uint32_t>(m_graphic.extensions.device_extensions.size());
	device_info.ppEnabledExtensionNames = m_graphic.extensions.device_extensions.data();

	if (m_graphic.validation_layer_enable)
	{
		device_info.enabledLayerCount = static_cast<uint32_t>(m_graphic.extensions.validationLayers.size());
		device_info.ppEnabledLayerNames = m_graphic.extensions.validationLayers.data();
	}
	else
	{
		device_info.enabledLayerCount = 0;
	}

	if (vkCreateDevice(m_graphic.physical_device, &device_info, nullptr, &m_graphic.device) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device!");
	}

	m_graphic.queue_indices.graphic_family = indices.graphic_family;
	m_graphic.queue_indices.present_family = indices.present_family;

	vkGetDeviceQueue(m_graphic.device, m_graphic.queue_indices.graphic_family, 0, &m_graphic.graphics_queue);
	vkGetDeviceQueue(m_graphic.device, m_graphic.queue_indices.present_family, 0, &m_graphic.present_queue);
}

bool VulkanDevice::checkDeviceSuitability(VkPhysicalDevice device)
{
	bool swapChainAdequate = false;

	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	if (!(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader))
	{
		return false;
	}

	QueueFamilyIndices indices = findQueueFamily(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	if (extensionsSupported)
	{
		SwapchainDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool VulkanDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extension_count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, availableExtensions.data());

/*#ifndef NDEBUG
	for (uint32_t i = 0; i < availableExtensions.size(); i++)
	{
		std::clog << availableExtensions[i].extensionName << "\t";
	}
#endif // !NDEBUG*/


	std::set<std::string> requiredExtensions(m_graphic.extensions.device_extensions.begin(), m_graphic.extensions.device_extensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

QueueFamilyIndices VulkanDevice::findQueueFamily(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

	int i = 0;
	for (const auto& queueFamily : queue_families)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphic_family = i;
		}

		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_graphic.surface, &present_support);

		if (queueFamily.queueCount > 0 && present_support)
		{
			indices.present_family = i;
		}

		if (indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

SwapchainDetails VulkanDevice::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapchainDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_graphic.surface, &details.capabilities);


	uint32_t format_count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_graphic.surface, &format_count, nullptr);

	if (format_count != 0)
	{
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_graphic.surface, &format_count, details.formats.data());
	}


	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_graphic.surface, &present_mode_count, nullptr);

	if (present_mode_count != 0)
	{
		details.presentModes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_graphic.surface, &present_mode_count, details.presentModes.data());
	}

	return details;
}
