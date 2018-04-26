#include "Device.h"

#include <iostream>

Device::Device(Renderer::Vulkan& vulkan) : m_vulkan(vulkan)
{
	uint32_t nbDevices = 0;
	if ((vkEnumeratePhysicalDevices(m_vulkan.instance, &nbDevices, nullptr) != VK_SUCCESS) || (nbDevices == 0))
	{
		throw std::runtime_error("No GPUs found!");
	}

	std::vector<VkPhysicalDevice> physicalDevices(nbDevices);
	if (vkEnumeratePhysicalDevices(m_vulkan.instance, &nbDevices, &physicalDevices[0]) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to acquire all GPUs!");
	}

	//check device properties
	VkPhysicalDevice selected_physical_device = VK_NULL_HANDLE;
	uint32_t selected_graphics_queue_family_index = UINT32_MAX;
	uint32_t selected_present_queue_family_index = UINT32_MAX;

	for (uint32_t i = 0; i < nbDevices; ++i)
	{
		if (checkPhysicalDeviceProperties(physicalDevices[i], selected_graphics_queue_family_index, selected_present_queue_family_index)) {
			selected_physical_device = physicalDevices[i];
		}
	}

	if (selected_physical_device == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find a suitable device!");
	}

	m_vulkan.physicalDevice = selected_physical_device;


	//create device
	std::vector<VkDeviceQueueCreateInfo> queue_info;
	std::vector<float> queue_priorities = { 1.0f };

	queue_info.push_back
	(
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr,
			0,
			selected_graphics_queue_family_index,
			static_cast<uint32_t>(queue_priorities.size()),
			&queue_priorities[0]
		}
	);

	if (selected_graphics_queue_family_index != selected_present_queue_family_index)
	{
		queue_info.push_back
		(
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				nullptr,
				0,
				selected_present_queue_family_index,
				static_cast<uint32_t>(queue_priorities.size()),
				&queue_priorities[0]
			}
		);
	}


	std::vector<const char*> extensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkDeviceCreateInfo device_info = {};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pNext = nullptr;
	device_info.flags = 0;

	device_info.queueCreateInfoCount = static_cast<uint32_t>(queue_info.size());
	device_info.pQueueCreateInfos = &queue_info[0];

	device_info.enabledLayerCount = 0;
	device_info.ppEnabledLayerNames = nullptr;

	device_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	device_info.ppEnabledExtensionNames = &extensions[0];

	device_info.pEnabledFeatures = nullptr;

	if (vkCreateDevice(selected_physical_device, &device_info, nullptr, &m_vulkan.device) != VK_SUCCESS)
	{
		throw std::runtime_error("Could not create Vulkan device!");
	}

	m_vulkan.graphics_queue_family_index = selected_graphics_queue_family_index;
	m_vulkan.present_queue_family_index = selected_present_queue_family_index;
}


Device::~Device()
{
}

bool Device::checkPhysicalDeviceProperties(VkPhysicalDevice & physical_devices, uint32_t graphics_queue_index, uint32_t present_queue_index)
{
	uint32_t extensions_count = 0;
	if (vkEnumerateDeviceExtensionProperties(physical_devices, nullptr, &extensions_count, nullptr) != VK_SUCCESS || extensions_count == 0)
	{
		std::cout << "Error occurred during physical device " << physical_devices << " extensions enumeration!" << std::endl;
		return false;
	}

	std::vector<VkExtensionProperties> available_extensions(extensions_count);
	if (vkEnumerateDeviceExtensionProperties(physical_devices, nullptr, &extensions_count, &available_extensions[0]))
	{
		std::cout << "Error occurred during physical device " << physical_devices << " extensions enumeration!" << std::endl;
		return false;
	}

	std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	for (size_t i = 0; i < device_extensions.size(); i++)
	{
		if (!checkExtensionsAvailability(device_extensions[i], available_extensions))
		{
			std::cout << "Physical device " << physical_devices << " doesn't support extension named \"" << device_extensions[i] << "\"!" << std::endl;
			return false;
		}
	}


	VkPhysicalDeviceProperties device_properties;
	VkPhysicalDeviceFeatures   device_features;

	vkGetPhysicalDeviceProperties(physical_devices, &device_properties);
	vkGetPhysicalDeviceFeatures(physical_devices, &device_features);

	uint32_t major_version = VK_VERSION_MAJOR(device_properties.apiVersion);
	uint32_t minor_version = VK_VERSION_MINOR(device_properties.apiVersion);
	uint32_t patch_version = VK_VERSION_PATCH(device_properties.apiVersion);

	if (major_version < 1)
	{
		std::cout << "Physical device " << physical_devices << " doesn't support required parameters!" << std::endl;
		return false;
	}


	uint32_t queue_families_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physical_devices, &queue_families_count, nullptr);

	if (queue_families_count == 0)
	{
		std::cout << "Physical device " << physical_devices << " doesn't have any queue families!" << std::endl;
		return false;
	}


	std::vector<VkQueueFamilyProperties> queue_family_properties(queue_families_count);
	std::vector<VkBool32> queue_present_support(queue_families_count);

	vkGetPhysicalDeviceQueueFamilyProperties(physical_devices, &queue_families_count, &queue_family_properties[0]);

	uint32_t temp_graphics_queue_family_index = UINT32_MAX;
	uint32_t temp_present_queue_family_index = UINT32_MAX;

	for (uint32_t i = 0; i < queue_families_count; ++i)
	{
		vkGetPhysicalDeviceSurfaceSupportKHR(physical_devices, i, m_vulkan.surface, &queue_present_support[i]);

		if ((queue_family_properties[i].queueCount > 0) && (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
		{
			if (temp_graphics_queue_family_index == UINT32_MAX)
			{
				temp_graphics_queue_family_index = i;
			}

			if (queue_present_support[i])
			{
				graphics_queue_index = i;
				present_queue_index = i;
				return true;
			}
		}
	}

	for (uint32_t i = 0; i < queue_families_count; ++i)
	{
		if (queue_present_support[i])
		{
			temp_present_queue_family_index = i;
			break;
		}
	}

	if ((temp_graphics_queue_family_index == UINT32_MAX) || (temp_present_queue_family_index == UINT32_MAX))
	{
		std::cout << "Could not find queue family with required properties on physical device " << physical_devices << "!" << std::endl;
		return false;
	}

	graphics_queue_index = temp_graphics_queue_family_index;
	present_queue_index = temp_present_queue_family_index;
	return true;
}

bool Device::checkExtensionsAvailability(const char* extensions, std::vector<VkExtensionProperties> available_extensions)
{
	for (size_t i = 0; i < available_extensions.size(); i++)
	{
		if (strcmp(available_extensions[i].extensionName, extensions) == 0)
		{
			return true;
		}
	}
	return false;
}
