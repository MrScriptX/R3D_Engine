#include "UniformBuffer.h"



UniformBuffer::UniformBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice) : m_device(device)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	Buffer::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffer, m_uniformBufferMemory, device, physicalDevice);
}


UniformBuffer::~UniformBuffer()
{
	vkDestroyBuffer(m_device, m_uniformBuffer, nullptr);
	vkFreeMemory(m_device, m_uniformBufferMemory, nullptr);
}

VkDeviceMemory & UniformBuffer::getBufferMemory()
{
	return m_uniformBufferMemory;
}

VkBuffer & UniformBuffer::getBuffer()
{
	return m_uniformBuffer;
}
