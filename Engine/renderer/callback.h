#ifndef VRED_RENDERER_CALLBACK_H
#define VRED_RENDERER_CALLBACK_H

#include <vulkan/vulkan.h>

namespace vred::renderer
{
	VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);
	VkResult create_debug_report_callback(VkInstance& instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
	void destroy_debug_report_callback(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
}

#endif // !VRED_RENDERER_CALLBACK_H
