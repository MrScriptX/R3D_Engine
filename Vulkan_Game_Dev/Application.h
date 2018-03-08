#pragma once


#include "Pipeline.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <memory>

#include "Buffer.h"
#include "DescriptorPool.h"
#include "DescriptorSet.h"
#include "DescriptorSetLayout.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
#include "Queue.h"
#include "UniformBuffer.h"
#include "RenderPass.h"
#include "FrameBuffer.h"
#include "Semaphore.h"



//use assert for abort and execption for report or recovery

const std::vector<const char*> validationLayers = { "VK_LAYER_LUNARG_standard_validation" };
const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


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
	void cleanSwapChain();

	void createInstance();//create a vulkan instance
	void createSurface();
	void createLogicalDevice();
	void createSwapChain();
	void recreateSwapChain();
	void createImageView();
	void createTextureImage();
	void createImage(uint32_t texWidth, uint32_t texHeight, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void setupCallBack();
	void pickPhysicalDevice();
	void updateUniformBuffer();

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkValidationLayerSupport();
	bool isDeviceSuitable(VkPhysicalDevice device);

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	std::vector<const char*> getRequiredExtensions();

	static void onWindowResized(GLFWwindow* window, int width, int height);
	static VkResult createDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
	static void destroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);


private:
	VkDebugReportCallbackEXT callback;
	VkSurfaceKHR m_surface;
	VkInstance m_instance;
	GLFWwindow * m_window;

	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;

	std::vector<VkImage> m_swapChainImages;
	std::vector<VkImageView> m_swapChainImageViews;
	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;
	VkSwapchainKHR m_swapChain;

	std::unique_ptr<Buffer> m_buffer;
	std::unique_ptr<UniformBuffer> m_uniformBuffer;

	std::unique_ptr<FrameBuffer> m_frameBuffer;
	std::unique_ptr<RenderPass> m_renderPass;
	std::unique_ptr<Pipeline> m_pipeline;

	CommandBuffer m_commandBuffer;
	std::unique_ptr<CommandPool> m_commandPool;

	std::unique_ptr<DescriptorPool> m_descriptorPool;
	std::unique_ptr<DescriptorSet> m_descriptorSet;
	std::unique_ptr<DescriptorSetLayout> m_descriptorSetLayout;

	std::unique_ptr<Semaphore> m_semaphore;

	VkImage m_textureImage;
	VkDeviceMemory m_textureImageMemory;
};

