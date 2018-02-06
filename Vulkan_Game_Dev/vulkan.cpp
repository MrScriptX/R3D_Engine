#include "vulkan.h"



vulkan::vulkan()
{
}


vulkan::~vulkan()
{
}

void vulkan::createInstance()
{
	VkApplicationInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	info.pNext = nullptr;
	info.pApplicationName = "Name of app";
	info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	info.pEngineName = "No Engine";//backend engine name (unity, Frostbite...)
	info.engineVersion = VK_MAKE_VERSION(1, 0, 0);//use app version if no engine
	info.apiVersion = VK_API_VERSION_1_0;
}

void vulkan::pickPhysicalDevice()
{
}

void vulkan::createLogicDevice()
{
}
