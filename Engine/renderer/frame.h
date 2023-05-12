#ifndef VRED_RENDERER_FRAME_H
#define VRED_RENDERER_FRAME_H

#include <vulkan/vulkan.h>

namespace vred::renderer
{
	struct frame
	{
		VkFence render_fence;
		VkSemaphore image_available;
		VkSemaphore render_finished;
		
		VkCommandBuffer main_command_buffer;
		VkCommandBuffer ui_command_buffer;
		
		VkFramebuffer main_framebuffer;
		VkFramebuffer ui_framebuffer;
	};
}

#endif // !VRED_RENDERER_FRAME_H
