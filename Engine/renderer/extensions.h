#ifndef VRED_RENDERER_EXTENSIONS_H
#define VRED_RENDERER_EXTENSIONS_H

#include <array>

namespace vred::renderer::extensions
{
	constexpr std::array<const char*, 2> validation_layers = { "VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_monitor" };
	constexpr std::array<const char*, 1> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
}

#endif // !VRED_RENDERER_EXTENSIONS_H
