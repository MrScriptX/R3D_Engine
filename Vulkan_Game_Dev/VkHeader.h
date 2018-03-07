#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>


struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentationFamily = -1;//added preFamily

	bool isComplete()
	{
		return graphicsFamily >= 0 && presentationFamily >= 0;
	}
};