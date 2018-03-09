#pragma once

#include "VkHeader.h"
#include "TextureView.h"
#include "Buffer.h"
#include "DescriptorSetLayout.h"
#include "Barrier.h"

class Texture
{
public:
	Texture(VkDevice& device, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue, VkCommandPool& commandPool);
	~Texture();

	static void createImage(uint32_t texWidth, uint32_t texHeight, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkPhysicalDevice& physicalDevice, VkDevice& device);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkQueue& graphicsQueue, VkCommandPool& commandPool);

	VkImage& getTextureImage();
private:

	VkDevice & m_device;
	VkImage m_textureImage;
	VkDeviceMemory m_textureImageMemory;
};

