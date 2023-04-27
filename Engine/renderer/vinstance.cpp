#include "vinstance.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include "extensions.h"
#include "parameters.h"

using namespace vred::renderer;

bool check_validation_layer_support();
std::vector<const char*> get_required_extensions();

VkInstance vred::renderer::create_instance()
{
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = nullptr;
	app_info.pApplicationName = "Engine";
	app_info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	if (parameters::validation_layer_enable && !check_validation_layer_support())
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkInstanceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.flags = 0;
	create_info.pApplicationInfo = &app_info;

	const std::vector<const char*> extensions = get_required_extensions();
	create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	create_info.ppEnabledExtensionNames = extensions.data();

	if (parameters::validation_layer_enable)
	{
		create_info.enabledLayerCount = static_cast<uint32_t>(extensions::validation_layers.size());
		create_info.ppEnabledLayerNames = extensions::validation_layers.data();
	}
	else
	{
		create_info.enabledLayerCount = 0;
	}

	VkInstance instance;
	if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
	{
		std::cerr << "Failed to create vulkan instance!" << std::endl;
	}

	return instance;
}

bool check_validation_layer_support()
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

	for (const char* layerName : extensions::validation_layers)
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

std::vector<const char*> get_required_extensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (parameters::validation_layer_enable)
	{
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}
