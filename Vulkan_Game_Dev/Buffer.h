#pragma once

#include "VkHeader.h"
#include "vertex.h"


class Buffer
{
public:
	Buffer();
	~Buffer();

	VkBuffer getVertexBuffer();
	VkBuffer getIndexBuffer();
	VkBuffer getUniformBuffer();
	VkDeviceMemory getUniformBufferMemory();

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice& device, VkPhysicalDevice& physicalDevice);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue);
	void clean(VkDevice& device);

	void createVertexBuffer(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkPhysicalDevice& physicalDevice);
	void createIndexBuffer(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkPhysicalDevice& physicalDevice);
	void createUniformBuffer(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkPhysicalDevice& physicalDevice);

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice& physicalDevice);
private:
	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;

	VkBuffer m_indexBuffer;
	VkDeviceMemory m_indexBufferMemory;

	VkBuffer m_uniformBuffer;
	VkDeviceMemory m_uniformBufferMemory;

	/*add reference to member var from App class instead of calling them declaring them in fonctions*/
};

