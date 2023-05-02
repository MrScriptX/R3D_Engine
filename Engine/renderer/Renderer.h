#ifndef R3DENGINE_RENDERER_H_
#define R3DENGINE_RENDERER_H_

#ifndef _GLFW3_
#define _GLFW3_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#endif // !_GLFW3_

#include <algorithm>
#include <bitset>
#include <iostream>
#include <memory>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include "VulkanBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanDescriptor.h"
#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"
#include "VulkanSwapchain.h"

#include "../graphics/CameraUBO.h"
#include "../graphics/Graphics.h"

#include "../Logger.h"

#include "interface.h"
#include "frame.h"

class Renderer
{
  public:
	Renderer(GLFWwindow& window, uint32_t width, uint32_t height);
	~Renderer();

	int32_t draw();
	void UpdateUI();
	int32_t AcquireNextImage();
	void WaitForSwapchainImageFence();

	void SetPolygonFillingMode(const VkPolygonMode& mode);
	void SetColorMode(const ColorMode map);

	const VkDevice& GetDevice();
	VkCommandBuffer& GetCommandBuffer(const size_t& i);
	VkFramebuffer& GetFrameBuffer(const size_t& i);
	std::unique_ptr<VulkanBuffer>& getBufferFactory();
	std::unique_ptr<VulkanPipeline>& GetPipelineFactory();

	// Track State
	void SetUpdated(const size_t& i);

	const bool IsUpdated();
	const bool NeedUpdate(const size_t& i);

	// Create Buffers
	void CreateVerticesBuffer(std::shared_ptr<std::vector<Vertex>> vertices, Buffer& buffer);
	void CreateIndicesBuffer(std::shared_ptr<std::vector<uint32_t>> indices, Buffer& buffer);
	void CreateUniformBuffer(VkBuffer& buffer, VkDeviceMemory& memory, VkDeviceSize size);

	// Record Command Buffer
	void BeginRecordCommandBuffers(VkCommandBuffer& commandBuffer, VkFramebuffer& frameBuffer);
	void EndRecordCommandBuffers(VkCommandBuffer& commandBuffer);

	// Ressource allocation and update
	void allocateDescriptorSet(VkDescriptorSet& descriptor_set);
	void allocateDescriptorSetLight(VkDescriptorSet& descriptor_set);
	void updateDescriptorSet(const VkBuffer& ubo, const VkDescriptorSet& descriptor_set, VkDeviceSize size);
	void updateDescriptorSet(const VkBuffer& ubo, const VkDescriptorSet& descriptor_set, const VkImageView& image_view, const VkSampler& image_sampler);

	// Create Image
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
	                 VkDeviceMemory& imageMemory);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	// Cleaning Ressource
	void destroyUniformBuffer();
	void destroyBuffers(Buffer& buffers);
	void cleanSwapchain();

  private:
	void setup_debug_callback(VkInstance& instance);
	void setupSwapchain();
	void setupRenderPass();
	void setupDescriptorSetLayout();
	void createCommandPool();
	void createFramebuffer();
	void createDescriptorPool();
	void allocateCommandBuffers();
	void createDepthResources();

	void recreateSwapchain();

	void initUI(GLFWwindow& window);

	SwapchainDetails querySwapChainSupport(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamily(VkPhysicalDevice device);

	// static fonctions
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code,
	                                                    const char* layerPrefix, const char* msg, void* userData);

	// proxy fonctions
	VkResult CreateDebugReportCallbackEXT(VkInstance& instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
	                                      VkDebugReportCallbackEXT* pCallback);
	void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);
	VkCommandBuffer beginCommands();
	void endCommands(VkCommandBuffer commandBuffer);

	VkDebugReportCallbackEXT callback;
	Graphics m_graphic;
	UIObject m_ui;
	std::unique_ptr<uint32_t> WIDTH;
	std::unique_ptr<uint32_t> HEIGHT;

	std::unique_ptr<VulkanSwapchain> m_swapchain;
	std::unique_ptr<VulkanRenderPass> m_pRenderpass;
	std::unique_ptr<VulkanDescriptor> m_descriptor;
	std::unique_ptr<VulkanCommandPool> m_commandPool;
	std::unique_ptr<VulkanPipeline> mp_pipelines_manager;
	std::unique_ptr<VulkanBuffer> m_pBufferFactory;

	uint32_t m_current_image = 0;
	uint32_t m_last_image = 0;
	std::bitset<3> m_is_updated;

	vred::renderer::interface m_interface;
	std::array<vred::renderer::frame, 3> m_frames;
};

#endif // !R3DENGINE_RENDERER_H_
