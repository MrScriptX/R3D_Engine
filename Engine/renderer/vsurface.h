#ifndef VRED_RENDERER_VSURFACE_H
#define VRED_RENDERER_VSURFACE_H

#include <vulkan/vulkan.h>

#ifndef _GLFW3_
#define _GLFW3_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#endif // !_GLFW3_

namespace vred::renderer
{
	VkSurfaceKHR create_surface(GLFWwindow& window, const VkInstance& instance);
}

#endif // !VRED_RENDERER_VSURFACE_H
