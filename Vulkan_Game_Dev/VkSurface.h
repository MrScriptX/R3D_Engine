#pragma once

#include "VkHeader.h"


class VkSurface
{
public:
	VkSurface(VkInstance& instance, GLFWwindow& window);
	~VkSurface();

	VkSurfaceKHR& getSurface();

private:

	VkInstance& m_instance;
	VkSurfaceKHR m_surface;
};

