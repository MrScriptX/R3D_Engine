#pragma once

#include "VkHeader.h"
#include "CommandBuffer.h"
#include "vertex.h"


class Buffer
{
public:
	Buffer(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkPhysicalDevice& physicalDevice, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	~Buffer();

	void bindVertexBuffer(std::vector<VkCommandBuffer>& commandBuffer);
	void bindIndexBuffer(std::vector<VkCommandBuffer>& commandBuffer);

	VkBuffer& getVertexBuffer();
	VkBuffer& getIndexBuffer();

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue);

	static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice& device, VkPhysicalDevice& physicalDevice);
	static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice& physicalDevice);
private:

	void createVertexBuffer(VkQueue& graphicsQueue, VkPhysicalDevice& physicalDevice, std::vector<Vertex>& vertices);
	void createIndexBuffer(VkQueue& graphicsQueue, VkPhysicalDevice& physicalDevice, std::vector<uint32_t>& indices);


	VkDevice& m_device;
	VkCommandPool& m_commandPool;

	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;

	VkBuffer m_indexBuffer;
	VkDeviceMemory m_indexBufferMemory;

	/*add reference to member var from App class instead of calling them declaring them in fonctions*/
};

