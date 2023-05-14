#ifndef R3DENGINE_VULKANBUFFER_H_
#define R3DENGINE_VULKANBUFFER_H_

#include <memory>
#include <stdexcept>

#include "../graphics/Graphics.h"

#include "ihardware.h"

class VulkanBuffer
{
  public:
	VulkanBuffer(vred::renderer::ihardware& hw);
	~VulkanBuffer();

	void createBuffer(VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

  private:
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	vred::renderer::ihardware& m_hw;
};

#endif // !R3DENGINE_VULKANBUFFER_H_
