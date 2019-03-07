#ifndef _VULKAN_COMMANDPOOL_H
#define _VULKAN_COMMANDPOOL_H

#include "Graphics.h"

class VulkanCommandPool
{
public:
	VulkanCommandPool(Graphics& m_graphic);
	~VulkanCommandPool();

	void createCommandPool();
private:

	Graphics & m_graphic;
};

#endif //!_VULKAN_COMMANDPOOL_H