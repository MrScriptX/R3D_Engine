#ifndef VRED_RENDERER_RENDER_H
#define VRED_RENDERER_RENDER_H

#include <vulkan/vulkan.h>

namespace vred::renderer
{
	struct render
	{
		VkDescriptorSetLayout main_descriptor_set_layout;
		VkDescriptorSetLayout light_descriptor_set_layout;

		VkDescriptorPool main_pool;
		VkDescriptorPool ui_pool;

		VkPolygonMode polygone_mode = VK_POLYGON_MODE_FILL;
		ColorMode color_map = ColorMode::DEFAULT;

		VkBuffer uniform_buffer;
		VkDeviceMemory uniform_memory;
	};
}

#endif // !VRED_RENDERER_FRAME_H
