#pragma once

#include "Common.h"

class Instance
{
public:
	Instance(Renderer::Vulkan& vulkan);
	~Instance();

private:
	
	bool checkAvailableExtensions(const char* extensions, std::vector<VkExtensionProperties> available_extensions);

	Renderer::Vulkan& m_vulkan;
};

