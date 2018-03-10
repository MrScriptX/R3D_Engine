#pragma once

#include "VkHeader.h"
#include "Texture.h"
#include "Barrier.h"

class DepthRessources
{
public:
	DepthRessources(uint32_t texWidth, uint32_t texHeight, VkDevice& device, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue);
	~DepthRessources();


	bool hasStencilComponent(VkFormat format);
	
	static VkFormat findSupportedFormat(const std::vector<VkFormat>& canditates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice& physicalDevice);
	static VkFormat findDepthFormat(VkPhysicalDevice& physicalDevice);

	VkImageView& getImageView();
private:

	VkDevice& m_device;
	VkPhysicalDevice& m_physicalDevice;

	VkImage m_depthImage;
	VkDeviceMemory m_depthImageMemory;
	VkImageView m_depthImageView;
};

