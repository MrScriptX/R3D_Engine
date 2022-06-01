#ifndef R3DENGINE_VULKAN_INSTANCE_H_
#define R3DENGINE_VULKAN_INSTANCE_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstring>
#include <iostream>

#include "../graphics/Graphics.h"

class VulkanInstance
{
  public:
	VulkanInstance(Graphics& m_graphic);
	~VulkanInstance();

  private:
	void createInstance();

	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();

	Graphics& m_graphic;
};

#endif //! R3DENGINE_VULKAN_INSTANCE_H_
