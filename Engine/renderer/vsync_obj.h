#ifndef VRED_RENDERER_VSYNC_OBJ_H
#define VRED_RENDERER_VSYNC_OBJ_H

#include <vulkan/vulkan.h>

namespace vred::renderer
{
	VkFence create_fence(const VkDevice& device);
	VkSemaphore create_semaphore(const VkDevice& device);
}

#endif // !VRED_RENDERER_VSYNC_OBJ_H
