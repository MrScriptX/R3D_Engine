#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>


class vulkan
{
public:
	vulkan();
	~vulkan();

	void createInstance();
	void pickPhysicalDevice();
	void createLogicDevice();
private:

};

