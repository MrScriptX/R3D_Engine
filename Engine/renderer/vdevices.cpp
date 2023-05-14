#include "vdevices.h"

#include <stdexcept>
#include <vector>
#include <set>
#include <iostream>

#include "queue_family.h"
#include "extensions.h"
#include "parameters.h"
#include "vswapchain.h"

using namespace vred::renderer;

bool check_device_suitability(const VkPhysicalDevice& device, const ihardware& hw);
queue_family_indices find_queue_family(const VkPhysicalDevice& device, const ihardware& hw);
bool check_device_extension_support(const VkPhysicalDevice& device);

VkPhysicalDevice vred::renderer::choose_device(const ihardware& hw)
{
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(hw.instance, &device_count, nullptr);

	if (device_count == 0)
		throw std::runtime_error("No physical device found !");

	std::vector<VkPhysicalDevice> available_device(device_count);
	vkEnumeratePhysicalDevices(hw.instance, &device_count, available_device.data());

	VkPhysicalDevice device = VK_NULL_HANDLE;
	for (uint32_t i = 0; i < available_device.size(); i++)
	{
		if (check_device_suitability(available_device[i], hw))
		{
			device = available_device[i];
			break;
		}
	}

	if (device == VK_NULL_HANDLE)
		throw std::runtime_error("No physical device has required specs !");

	VkPhysicalDeviceProperties device_properties;
	vkGetPhysicalDeviceProperties(device, &device_properties);

	std::clog << "Physical Device Name: " << device_properties.deviceName << "\n";
	std::clog << "Physical Device Vendor ID: " << device_properties.vendorID << "\n";
	std::clog << "Physical Device Driver version: " << device_properties.driverVersion << std::endl;

	return device;
}

void vred::renderer::create_device_interface(ihardware& hw)
{
	float queuePriority = 1.0f;
	queue_family_indices indices = find_queue_family(hw.physical_device, hw);
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
	device_info.enabledExtensionCount = static_cast<uint32_t>(extensions::device_extensions.size());
	device_info.ppEnabledExtensionNames = extensions::device_extensions.data();

	if (parameters::validation_layer_enable)
	{
		device_info.enabledLayerCount = static_cast<uint32_t>(extensions::validation_layers.size());
		device_info.ppEnabledLayerNames = extensions::validation_layers.data();
	}
	else
	{
		device_info.enabledLayerCount = 0;
	}

	if (vkCreateDevice(hw.physical_device, &device_info, nullptr, &hw.device) != VK_SUCCESS)
		throw std::runtime_error("Failed to create logical device!");

	hw.queue_indices.graphic_family = indices.graphic_family;
	hw.queue_indices.present_family = indices.present_family;

	vkGetDeviceQueue(hw.device, indices.graphic_family, 0, &hw.graphics_queue);
	vkGetDeviceQueue(hw.device, indices.present_family, 0, &hw.present_queue);
}

bool check_device_suitability(const VkPhysicalDevice& device, const ihardware& hw)
{
	VkPhysicalDeviceProperties device_properties;
	vkGetPhysicalDeviceProperties(device, &device_properties);

	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures(device, &device_features);

	if (device_properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && device_properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
	{
		std::clog << "device type does not match\n";
		return false;	
	}

	if (!device_features.geometryShader)
	{
		std::clog << "geometry shader is not supported\n";
		return false;	
	}

	bool swapchain_adequate = false;
	queue_family_indices indices = find_queue_family(device, hw);
	bool extensions_supported = check_device_extension_support(device);

	if (extensions_supported)
	{
		swapchain_details swapChainSupport = query_swapchain_support(device, hw);
		swapchain_adequate = !swapChainSupport.formats.empty() && !swapChainSupport.present_modes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.is_complete() && extensions_supported && swapchain_adequate && supportedFeatures.samplerAnisotropy;
}

queue_family_indices find_queue_family(const VkPhysicalDevice& device, const ihardware& hw)
{
	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

	int i = 0;
	queue_family_indices indices;
	for (const auto& queueFamily : queue_families)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphic_family = i;
		}

		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, hw.surface, &present_support);

		if (queueFamily.queueCount > 0 && present_support)
		{
			indices.present_family = i;
		}

		if (indices.is_complete())
		{
			break;
		}

		i++;
	}

	return indices;
}

bool check_device_extension_support(const VkPhysicalDevice& device)
{
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> available_extensions(extension_count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

	/*#ifndef NDEBUG
	    for (uint32_t i = 0; i < availableExtensions.size(); i++)
	    {
	        std::clog << availableExtensions[i].extensionName << "\t";
	    }
	#endif // !NDEBUG*/

	std::set<std::string> required_extensions(extensions::device_extensions.begin(), extensions::device_extensions.end());
	for (const auto& extension : available_extensions)
	{
		required_extensions.erase(extension.extensionName);
	}

	return required_extensions.empty();
}
