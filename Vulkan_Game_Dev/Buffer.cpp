#include "Buffer.h"



Buffer::Buffer(VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkPhysicalDevice& physicalDevice, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) : m_device(device), m_commandPool(commandPool)
{
	createVertexBuffer(graphicsQueue, physicalDevice, vertices);
	createIndexBuffer(graphicsQueue, physicalDevice, indices);
}


Buffer::~Buffer()
{
	vkDestroyBuffer(m_device, m_indexBuffer, nullptr);
	vkFreeMemory(m_device, m_indexBufferMemory, nullptr);
	vkDestroyBuffer(m_device, m_vertexBuffer, nullptr);
	vkFreeMemory(m_device, m_vertexBufferMemory, nullptr);
}

void Buffer::bindVertexBuffer(std::vector<VkCommandBuffer>& commandBuffer)
{
	for (size_t i = 0; i < commandBuffer.size(); i++)
	{
		VkBuffer vertexBuffers[] = { m_vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer[i], 0, 1, vertexBuffers, offsets);
	}
}

void Buffer::bindIndexBuffer(std::vector<VkCommandBuffer>& commandBuffer)
{
	for (size_t i = 0; i < commandBuffer.size(); i++)
	{
		vkCmdBindIndexBuffer(commandBuffer[i], m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
}

VkBuffer& Buffer::getVertexBuffer()
{
	return m_vertexBuffer;
}

VkBuffer& Buffer::getIndexBuffer()
{
	return m_indexBuffer;
}

void Buffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue)
{
	VkCommandBuffer commandBuffer = CommandBuffer::beginSingleTimeCommands(device, commandPool);

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	CommandBuffer::endSingleTimeCommands(device, commandPool, graphicsQueue, commandBuffer);
}

void Buffer::createVertexBuffer(VkQueue& graphicsQueue, VkPhysicalDevice& physicalDevice, std::vector<Vertex>& vertices)
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingMem;
	VkDeviceMemory stagingBufferMem;
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingMem, stagingBufferMem, m_device, physicalDevice);

	void * data;
	vkMapMemory(m_device, stagingBufferMem, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_device, stagingBufferMem);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory, m_device, physicalDevice);

	copyBuffer(stagingMem, m_vertexBuffer, bufferSize, m_device, m_commandPool, graphicsQueue);

	vkDestroyBuffer(m_device, stagingMem, nullptr);
	vkFreeMemory(m_device, stagingBufferMem, nullptr);
}

void Buffer::createIndexBuffer(VkQueue& graphicsQueue, VkPhysicalDevice& physicalDevice, std::vector<uint32_t>& indices)
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingMem;
	VkDeviceMemory stagingBufferMem;
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingMem, stagingBufferMem, m_device, physicalDevice);

	void* data;
	vkMapMemory(m_device, stagingBufferMem, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_device, stagingBufferMem);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory, m_device, physicalDevice);

	copyBuffer(stagingMem, m_indexBuffer, bufferSize, m_device, m_commandPool, graphicsQueue);

	vkDestroyBuffer(m_device, stagingMem, nullptr);
	vkFreeMemory(m_device, stagingBufferMem, nullptr);
}




/**************************************************************Static fonction********************************************************************/
/*************************************************************************************************************************************************/

void Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer & buffer, VkDeviceMemory & bufferMemory, VkDevice & device, VkPhysicalDevice& physicalDevice)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice & physicalDevice)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	std::cerr << "Error";
	throw std::runtime_error("failed to find suitable memory type!");
}
