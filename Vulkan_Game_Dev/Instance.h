#pragma once

#include "Common.h"
#include "ValidationLayers.h"

class Instance
{
public:
	Instance(Renderer::Vulkan& vulkan);
	~Instance();

	void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
	VkDebugReportCallbackEXT& getCallback();

private:
	
	bool checkAvailableExtensions(const char* extensions, std::vector<VkExtensionProperties> available_extensions);
	void setupDebugCallback();

	VkDebugReportCallbackEXT callback;
	Renderer::Vulkan& m_vulkan;
};

