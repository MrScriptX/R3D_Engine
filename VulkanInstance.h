#ifndef _VULKAN_INSTANCE_H
#define _VULKAN_INSTANCE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#include <iostream>
#include "Graphics.h"

class VulkanInstance
{
public:
	VulkanInstance(Graphics & m_graphic);
	~VulkanInstance();

private:
	void createInstance();

	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();

	Graphics & m_graphic;
};

#endif //!_VULKAN_INSTANCE_H