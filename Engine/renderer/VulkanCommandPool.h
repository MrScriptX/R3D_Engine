#ifndef R3DENGINE_VULKAN_COMMANDPOOL_H
#define R3DENGINE_VULKAN_COMMANDPOOL_H

#include <stdexcept>

#include "../graphics/Graphics.h"

class VulkanCommandPool
{
  public:
	VulkanCommandPool(Graphics& m_graphic);
	~VulkanCommandPool() = default;

	void CreateCommandPool(VkCommandPool& pool);

  private:
	Graphics& m_graphic;
};

#endif // !R3DENGINE_VULKAN_COMMANDPOOL_H
