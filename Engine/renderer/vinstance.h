#ifndef VRED_RENDERER_VINSTANCE_H
#define VRED_RENDERER_VINSTANCE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

namespace vred::renderer
{
	VkInstance create_instance();
}

#endif // !VRED_RENDERER_VINSTANCE_H
