#ifndef VRED_RENDERER_FRAME_H
#define VRED_RENDERER_FRAME_H

#include <vulkan/vulkan.h>

namespace vred::renderer
{
	struct frame
	{
		VkFence render_fence;
		VkCommandBuffer command_buffer;
	};
}

#endif // !VRED_RENDERER_FRAME_H
