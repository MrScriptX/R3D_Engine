#ifndef _RENDERER_H
#define _RENDERER_H

#ifndef _GLFW3_
#define _GLFW3_

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#endif // !_GLFW3_


#include <iostream>
#include <memory>
#include <algorithm>

#include "Voxel.h"

#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderPass.h"
#include "VulkanDescriptor.h"
#include "VulkanCommandPool.h"
#include "VulkanPipeline.h"
#include "VulkanBuffer.h"
#include "Graphics.h"

#include "Logger.h"

constexpr unsigned int MAX_FRAMES_IN_FLIGHT = 2;

class Renderer
{
public:
	Renderer(GLFWwindow& window, uint32_t width, uint32_t height);
	~Renderer();

	int32_t draw(Pipeline& pipeline);

	void setupInstance(GLFWwindow& window);
	void setupDevice();
	void setupSwapchain();
	void setupRenderPass();
	void setupDescriptorSetLayout();
	void setupCommandPool();
	void createNewPipeline(Pipeline& pipeline);
	
	void recordCommandBuffers(Pipeline& pipeline, size_t indices, Buffer& buffer);

	VkDevice& getDevice();
	Graphics& getGraphic();//need to seperate object needed outside the class


	//rendering

	void createVerticesBuffer(std::shared_ptr<std::vector<Voxel::Block>> vertices, Buffer& buffer);
	void createIndicesBuffer(std::shared_ptr<std::vector<uint16_t>> indices, Buffer& buffer);
	void createUBO();

	void allocateCommandBuffers();
	void beginRecordCommandBuffers(VkCommandBuffer& commandBuffer, VkFramebuffer& frameBuffer, Pipeline& pipeline);
	void recordDrawCommands(VkCommandBuffer& commandBuffer, Pipeline& pipeline, Buffer& buffer, size_t indices);
	void endRecordCommandBuffers(VkCommandBuffer& commandBuffer);

	//!rendering
	void setupCallback();
	void createSurface(GLFWwindow& window);
	void createFramebuffer();
	void createCommandPool();
	void createSyncObject();
	void createDescriptorLayout();
	void createDescriptorPool();
	void createDescriptorSet();
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
	void createDepthResources();

	//cleaning
	void destroyTextures();
	void destroyDescriptors();
	void destroyUniformBuffer();
	void destroyBuffers(Buffer& buffers);
	void cleanSwapchain(std::shared_ptr<Pipeline> pPipeline);

private:
	//init fonctions
	void recreateSwapchain(Pipeline& pipeline);
	

	//helper fonctions
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkDeviceSuitability(VkPhysicalDevice device);
	bool checkValidationLayerSupport();

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);

	VkCommandBuffer beginCommands();
	void endCommands(VkCommandBuffer commandBuffer);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	SwapchainDetails querySwapChainSupport(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamily(VkPhysicalDevice device);
	std::vector<const char*> getRequiredExtensions();

	//static fonctions
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
		uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);
	
	//proxy fonctions
	VkResult CreateDebugReportCallbackEXT(VkInstance& instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
	void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);


	//attributes

	VkDebugReportCallbackEXT callback;
	Graphics m_graphic;
	std::unique_ptr<uint32_t> WIDTH;
	std::unique_ptr<uint32_t> HEIGHT;


	std::unique_ptr<VulkanInstance> m_instance;
	std::unique_ptr<VulkanDevice> m_device;
	std::unique_ptr<VulkanSwapchain> m_swapchain;
	std::unique_ptr<VulkanRenderPass> m_pRenderpass;
	std::unique_ptr<VulkanDescriptor> m_descriptor;
	std::unique_ptr<VulkanCommandPool> m_commandPool;
	std::unique_ptr<VulkanPipeline> m_pPipelineFactory;
	std::unique_ptr<VulkanBuffer> m_pBufferFactory;

	size_t m_frame_index = 0;
};

#endif _RENDERER_H