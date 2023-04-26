#include "vdevices.h"

#include <stdexcept>
#include <vector>
#include <set>

#include "queue_family.h"
#include "extensions.h"
#include "swapchain_details.h"

using namespace vred::renderer;

bool checkDeviceSuitability(const VkPhysicalDevice& device, const interface& o);
queue_family_indices findQueueFamily(const VkPhysicalDevice& device, const interface& o);
bool checkDeviceExtensionSupport(const VkPhysicalDevice& device);
swapchain_details querySwapChainSupport(const VkPhysicalDevice& device, const interface& o);

VkPhysicalDevice vred::renderer::choose_device(const interface& o)
{
	uint32_t device_count = 0;
	vkEnumeratePhysicalDevices(o.instance, &device_count, nullptr);

	if (device_count == 0)
		throw std::runtime_error("No physical device found !");

	std::vector<VkPhysicalDevice> available_device(device_count);
	vkEnumeratePhysicalDevices(o.instance, &device_count, available_device.data());

	for (uint32_t i = 0; i < available_device.size(); i++)
	{
		if (checkDeviceSuitability(available_device[i], o))
		{
			return available_device[i];
		}
	}

	return VK_NULL_HANDLE;
}

bool checkDeviceSuitability(const VkPhysicalDevice& device, const interface& o)
{
	VkPhysicalDeviceProperties device_properties;
	vkGetPhysicalDeviceProperties(device, &device_properties);

	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures(device, &device_features);

	if (!(device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && device_features.geometryShader))
	{
		return false;
	}

	bool swapchain_adequate = false;
	queue_family_indices indices = findQueueFamily(device, o);
	bool extensions_supported = checkDeviceExtensionSupport(device);

	if (extensions_supported)
	{
		swapchain_details swapChainSupport = querySwapChainSupport(device, o);
		swapchain_adequate = !swapChainSupport.formats.empty() && !swapChainSupport.present_modes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.is_complete() && extensions_supported && swapchain_adequate && supportedFeatures.samplerAnisotropy;
}

queue_family_indices findQueueFamily(const VkPhysicalDevice& device, const interface& o)
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

bool checkDeviceExtensionSupport(const VkPhysicalDevice& device, const interface& o)
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

swapchain_details querySwapChainSupport(const VkPhysicalDevice& device, const interface& o)
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
