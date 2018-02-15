#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>


const std::vector<const char*> validationLayers = { "VK_LAYER_LUNARG_standard_validation" };

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


struct QueueFamilyIndices
{
	int graphicsFamily = -1;

	bool isComplete()
	{
		return graphicsFamily >= 0;
	}
};

class Application
{
public:
	Application();
	~Application();

	void run();
	void mainLoop();
	void clean();
	
	void initWindow();
	void initVulkan();

	void createInstance();
	void setupCallBack();
	void pickPhysicalDevice();
	void createLogicalDevice();

	bool checkValidationLayerSupport();
	bool isDeviceSuitable(VkPhysicalDevice device);

	std::vector<const char*> getRequiredExtensions();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	static VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
	static void destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);

private:
	VkDebugReportCallbackEXT callback;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device;
	VkQueue m_graphicsQueue;
	VkInstance m_instance;
	GLFWwindow * m_window;
};

