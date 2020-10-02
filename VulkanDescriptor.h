#ifndef _VULKAN_DESCRIPTOR_H
#define _VULKAN_DESCRIPTOR_H

#include <array>
#include <stdexcept>

#include "Vertex.h"
#include "Graphics.h"

class VulkanDescriptor
{
public:
	VulkanDescriptor(Graphics & graphic);
	~VulkanDescriptor();

	void createDescriptorSetLayout();
	void createDescriptorPool();
	void createDescriptorSet();

	void bindDescriptorSet(VkCommandBuffer& commandBuffer, VkPipelineLayout& pipelineLayout, VkDescriptorSet& descriptorSet);

private:

	Graphics & m_graphic;
};

#endif // !_VULKAN_DESCRIPTOR_H