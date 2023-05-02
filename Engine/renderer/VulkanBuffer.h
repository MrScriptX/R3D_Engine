#ifndef R3DENGINE_VULKANBUFFER_H_
#define R3DENGINE_VULKANBUFFER_H_

#include <memory>
#include <stdexcept>

#include "../graphics/Graphics.h"

class VulkanBuffer
{
  public:
	VulkanBuffer(Graphics& graphic);
	~VulkanBuffer();

	void createBuffer(VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

  private:
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	Graphics& m_graphic;
};

#endif // !R3DENGINE_VULKANBUFFER_H_
