#include "VulkanInstance.h"



VulkanInstance::VulkanInstance(Graphics & m_graphic) : m_graphic(m_graphic)
{
	createInstance();
}


VulkanInstance::~VulkanInstance()
{
}

void VulkanInstance::createInstance()
{
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = nullptr;
	app_info.pApplicationName = "Engine";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	if (m_graphic.validation_layer_enable && !checkValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkInstanceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.flags = 0;
	create_info.pApplicationInfo = &app_info;

	std::vector<const char*> extensions = getRequiredExtensions();
	create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	create_info.ppEnabledExtensionNames = extensions.data();

	if (m_graphic.validation_layer_enable)
	{
		create_info.enabledLayerCount = static_cast<uint32_t>(m_graphic.extensions.validationLayers.size());
		create_info.ppEnabledLayerNames = m_graphic.extensions.validationLayers.data();
	}
	else
	{
		create_info.enabledLayerCount = 0;
	}


	if (vkCreateInstance(&create_info, nullptr, &m_graphic.instance) != VK_SUCCESS)
	{
		std::cerr << "Failed to create vulkan instance!" << std::endl;
	}
}

bool VulkanInstance::checkValidationLayerSupport()
{
	uint32_t layer_count = 0;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	std::vector<VkLayerProperties> available_layers(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

	/*std::cout << "available layer:" << std::endl;
	for (const auto& extension : available_layers)
	{
	std::cout << "\t" << extension.layerName << std::endl;
	}*/

	for (const char* layerName : m_graphic.extensions.validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : available_layers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

std::vector<const char*> VulkanInstance::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (m_graphic.validation_layer_enable)
	{
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}
