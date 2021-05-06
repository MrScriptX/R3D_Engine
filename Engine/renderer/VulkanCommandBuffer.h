#ifndef _VULKAN_COMMANDBUFFER_H
#define _VULKAN_COMMANDBUFFER_H

#include "../graphics/Graphics.h"
#include <stdexcept>

class VulkanCommandBuffer
{
public:
	VulkanCommandBuffer(Graphics& m_graphic);
	~VulkanCommandBuffer();

	void allocateCommandBuffer();
	void recordCommandBuffer();

private:

	Graphics & m_graphic;
};

#endif // !_VULKAN_COMMANDBUFFER_H