#pragma once

#include "VkHeader.h"
#include "vertex.h"


class Buffer
{
public:
	Buffer(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkPhysicalDevice& physicalDevice);
	~Buffer();

	VkBuffer& getVertexBuffer();
	VkBuffer& getIndexBuffer();

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue);
	void clean(VkDevice& device);

	static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice& device, VkPhysicalDevice& physicalDevice);
	static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice& physicalDevice);
private:

	void createVertexBuffer(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkPhysicalDevice& physicalDevice);
	void createIndexBuffer(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkPhysicalDevice& physicalDevice);


	VkDevice& m_device;

	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;

	VkBuffer m_indexBuffer;
	VkDeviceMemory m_indexBufferMemory;

	/*add reference to member var from App class instead of calling them declaring them in fonctions*/
};

