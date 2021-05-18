#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "vulkan/vulkan.h"
#include <vector>

constexpr unsigned int MAX_FRAMES_IN_FLIGHT = 3;

struct Extensions
{
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_monitor" };
	const std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};

struct QueueFamilyIndices
{
	int32_t graphic_family = -1;
	int32_t present_family = -1;

	bool isComplete()
	{
		return graphic_family >= 0 && present_family >= 0;
	}
};

struct SwapchainDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct SwapchainImage
{
	VkFormat format;
	VkExtent2D extent;
};

struct Graphics
{
#ifdef NDEBUG
	const bool validation_layer_enable = false;
#else
	const bool validation_layer_enable = true;
#endif

	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkSurfaceKHR surface = VK_NULL_HANDLE;

	Extensions extensions;
	QueueFamilyIndices queue_indices;

	VkQueue graphics_queue = VK_NULL_HANDLE;
	VkQueue present_queue = VK_NULL_HANDLE;

	SwapchainImage swapchain_details;
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	std::vector<VkImage> swapchain_images;
	std::vector<VkImageView> images_view;

	std::vector<VkFramebuffer> framebuffers;

	VkCommandPool command_pool;
	std::vector<VkCommandBuffer> command_buffers;//replace VkCommandBuffer with CommandBuffer

	std::vector<VkSemaphore> semaphores_render_finished;
	std::vector<VkSemaphore> semaphores_image_available;
	std::vector<VkFence> fences_in_flight;

	VkRenderPass render_pass;

	VkBuffer uniform_buffer;
	VkDeviceMemory uniform_memory;

	VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
	VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;

	VkImage depth_image;
	VkDeviceMemory depth_memory;
	VkImageView depth_view;

	VkPolygonMode polygone_mode = VK_POLYGON_MODE_FILL;
};

struct Pipeline
{
	VkPipeline handle = VK_NULL_HANDLE;
	VkPipelineLayout layout = VK_NULL_HANDLE;
};

struct Buffer
{
	VkBuffer vertex = VK_NULL_HANDLE;
	VkDeviceMemory vertex_memory = VK_NULL_HANDLE;

	VkBuffer index = VK_NULL_HANDLE;
	VkDeviceMemory index_memory = VK_NULL_HANDLE;
};

struct CommandBuffers
{
	bool needUpdate = true;//base state
	VkCommandBuffer handle = VK_NULL_HANDLE;
};

#endif // !_GRAPHICS_H