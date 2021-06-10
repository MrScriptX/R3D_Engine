#include "../Includes/renderer/VulkanBuffer.h"

VulkanBuffer::VulkanBuffer(Graphics& graphic) : m_graphic(graphic)
{
}

VulkanBuffer::~VulkanBuffer()
{
}

void VulkanBuffer::createBuffer(VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	VkResult result;

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	result = vkCreateBuffer(m_graphic.device, &bufferInfo, nullptr, &buffer);
	switch (result)
	{
	case VK_SUCCESS:
		break;
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		throw std::runtime_error("Failed to create buffer!");
		break;
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		throw std::runtime_error("Failed to create buffer!");
		break;
	default:
		throw std::runtime_error("Failed to create buffer!");
		break;
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_graphic.device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	result = vkAllocateMemory(m_graphic.device, &allocInfo, nullptr, &bufferMemory);
	switch (result)
	{
	case VK_SUCCESS:
		break;
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		throw std::runtime_error("Failed to allocate buffer memory!");
		break;
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		throw std::runtime_error("Failed to allocate buffer memory!");
		break;
	case VK_ERROR_TOO_MANY_OBJECTS:
		throw std::runtime_error("Failed to allocate buffer memory!");
		break;
	case VK_ERROR_INVALID_EXTERNAL_HANDLE:
		throw std::runtime_error("Failed to allocate buffer memory!");
		break;

	default:
		throw std::runtime_error("Failed to allocate buffer memory!");
		break;
	}

	result = vkBindBufferMemory(m_graphic.device, buffer, bufferMemory, 0);
	switch (result)
	{
	case VK_SUCCESS:
		break;
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		throw std::runtime_error("Failed to bind buffer!");
		break;
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		throw std::runtime_error("Failed to bind buffer!");
		break;
	default:
		throw std::runtime_error("Failed to bind buffer!");
		break;
	}
}

uint32_t VulkanBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_graphic.physical_device, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}
