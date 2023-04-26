#include "vdevices.h"

#include <stdexcept>
#include <vector>
#include <set>
#include <iostream>

#include "queue_family.h"
#include "extensions.h"
#include "parameters.h"
#include "swapchain_details.h"

using namespace vred::renderer;

bool check_device_suitability(const VkPhysicalDevice& device, const interface& o);
queue_family_indices find_queue_family(const VkPhysicalDevice& device, const interface& o);
bool check_device_extension_support(const VkPhysicalDevice& device);
swapchain_details query_swapchain_support(const VkPhysicalDevice& device, const interface& o);

VkPhysicalDevice vred::renderer::choose_device(const interface& o)
{
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(o.instance, &device_count, nullptr);

	if (device_count == 0)
		throw std::runtime_error("No physical device found !");

	std::vector<VkPhysicalDevice> available_device(device_count);
	vkEnumeratePhysicalDevices(o.instance, &device_count, available_device.data());

	VkPhysicalDevice device = VK_NULL_HANDLE;
	for (uint32_t i = 0; i < available_device.size(); i++)
	{
		if (check_device_suitability(available_device[i], o))
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

void vred::renderer::create_device_interface(interface& o)
{
	float queuePriority = 1.0f;
	queue_family_indices indices = find_queue_family(o.physical_device, o);
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

	if (vkCreateDevice(o.physical_device, &device_info, nullptr, &o.device) != VK_SUCCESS)
		throw std::runtime_error("Failed to create logical device!");

	o.queue_indices.graphic_family = indices.graphic_family;
	o.queue_indices.present_family = indices.present_family;

	vkGetDeviceQueue(o.device, indices.graphic_family, 0, &o.graphics_queue);
	vkGetDeviceQueue(o.device, indices.present_family, 0, &o.present_queue);
}

bool check_device_suitability(const VkPhysicalDevice& device, const interface& o)
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
	queue_family_indices indices = find_queue_family(device, o);
	bool extensions_supported = check_device_extension_support(device);

	if (extensions_supported)
	{
		swapchain_details swapChainSupport = query_swapchain_support(device, o);
		swapchain_adequate = !swapChainSupport.formats.empty() && !swapChainSupport.present_modes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.is_complete() && extensions_supported && swapchain_adequate && supportedFeatures.samplerAnisotropy;
}

queue_family_indices find_queue_family(const VkPhysicalDevice& device, const interface& o)
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
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, o.surface, &present_support);

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

swapchain_details query_swapchain_support(const VkPhysicalDevice& device, const interface& o)
{
	swapchain_details details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, o.surface, &details.capabilities);

	uint32_t format_count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, o.surface, &format_count, nullptr);

	if (format_count != 0)
	{
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, o.surface, &format_count, details.formats.data());
	}

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, o.surface, &present_mode_count, nullptr);

	if (present_mode_count != 0)
	{
		details.present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, o.surface, &present_mode_count, details.present_modes.data());
	}

	return details;
}
