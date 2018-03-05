#pragma once


#include "Pipeline.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>


const std::vector<const char*> validationLayers = { "VK_LAYER_LUNARG_standard_validation" };
const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };//

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentationFamily = -1;//added preFamily

	bool isComplete()
	{
		return graphicsFamily >= 0 && presentationFamily >= 0;
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class Application
{
public:
	Application();
	~Application();

	void run();
	void mainLoop();
	void drawFrame();
	void clean();
	
	void initWindow();
	void initVulkan();

	void createInstance();//create a vulkan instance
	void createSurface();
	void createLogicalDevice();
	void createSwapChain();
	void createImageView();
	void createFrameBuffer();
	void createCommandPool();
	void createCommandBuffer();
	void createSemaphore();

	void setupCallBack();
	void pickPhysicalDevice();

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkValidationLayerSupport();
	bool isDeviceSuitable(VkPhysicalDevice device);

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	std::vector<const char*> getRequiredExtensions();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	static VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
	static void destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);


private:
	VkDebugReportCallbackEXT callback;
	VkSurfaceKHR m_surface;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device;
	VkQueue m_graphicsQueue;
	VkInstance m_instance;
	GLFWwindow * m_window;
	VkQueue m_presentQueue;

	std::vector<VkImage> m_swapChainImages;
	std::vector<VkImageView> m_swapChainImageViews;
	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;
	VkSwapchainKHR m_swapChain;

	Pipeline m_pipeline;

	//new
	std::vector<VkFramebuffer> m_swapChainFrameBuffer;
	std::vector<VkCommandBuffer> m_commandBuffer;
	VkCommandPool m_commandPool;

	VkSemaphore m_imageAvailable;
	VkSemaphore m_renderFinished;
};

