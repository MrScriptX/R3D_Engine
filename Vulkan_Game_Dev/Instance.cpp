#include "Instance.h"



Instance::Instance(Renderer::Vulkan& vulkan) : m_vulkan(vulkan)
{
	if (enableValidationLayers && !ValidationLayers::checkValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}


	uint32_t extensions_count = 0;
	if (vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr) != VK_SUCCESS)
	{
		throw std::runtime_error("Could not enumerate instance extensions!");
	}

	std::vector<VkExtensionProperties> available_extensions(extensions_count);
	if (vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, &available_extensions[0]) != VK_SUCCESS)
	{
		throw std::runtime_error("Could not get instance extensions");
	}

	std::vector<const char*> extensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_REPORT_EXTENSION_NAME };
	for (size_t i = 0; i < extensions.size(); i++)
	{
		if (!checkAvailableExtensions(extensions[i], available_extensions))
		{
			throw std::runtime_error("Could not find fitting instance extensions");
		}
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;

	appInfo.pApplicationName = "Vulkan Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

	appInfo.pEngineName = "R3D Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

	appInfo.apiVersion = VK_API_VERSION_1_0;


	VkInstanceCreateInfo instInfo = {};
	instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instInfo.pNext = nullptr;
	instInfo.flags = 0;
	instInfo.pApplicationInfo = &appInfo;

	instInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	instInfo.ppEnabledExtensionNames = &extensions[0];

	if (enableValidationLayers)
	{
		instInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		instInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		instInfo.enabledLayerCount = 0;
		instInfo.ppEnabledLayerNames = nullptr;
	}

	if (vkCreateInstance(&instInfo, nullptr, &m_vulkan.instance))
	{
		throw std::runtime_error("Failed to create Instance!");
	}

	setupDebugCallback();
}


Instance::~Instance()
{
}

bool Instance::checkAvailableExtensions(const char * extensions, std::vector<VkExtensionProperties> available_extensions)
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

void Instance::setupDebugCallback()
{
	if (!enableValidationLayers) return;

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = ValidationLayers::debugCallback;

	if (ValidationLayers::CreateDebugReportCallbackEXT(m_vulkan.instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug callback!");
	}
}

void Instance::DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

	if (func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}

VkDebugReportCallbackEXT& Instance::getCallback()
{
	return callback;
}
