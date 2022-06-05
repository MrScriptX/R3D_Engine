#ifndef R3DENGINE_VULKAN_COMMANDBUFFER_H_
#define R3DENGINE_VULKAN_COMMANDBUFFER_H_

#include "../graphics/Graphics.h"
#include <stdexcept>

class VulkanCommandBuffer
{
  public:
	VulkanCommandBuffer(Graphics& m_graphic);
	~VulkanCommandBuffer();

	void allocateCommandBuffer();

  private:
	Graphics& m_graphic;
};

#endif // !R3DENGINE_VULKAN_COMMANDBUFFER_H_
