#ifndef _VULKAN_DESCRIPTOR_H
#define _VULKAN_DESCRIPTOR_H

#include <array>
#include <stdexcept>

#include "../graphics/Vertex.h"
#include "../graphics/Graphics.h"

class VulkanDescriptor
{
public:
	VulkanDescriptor(Graphics & graphic);
	~VulkanDescriptor();

	void createDescriptorPool();

	void createDescriptorSetLayout();

	void createDescriptorSet(const VkDescriptorPool& descriptor_pool, VkDescriptorSetLayout& layout, VkDescriptorSet& descriptor_set);
	void updateDescriptorSet();

	void bindDescriptorSet(VkCommandBuffer& commandBuffer, VkPipelineLayout& pipelineLayout, VkDescriptorSet& descriptorSet);

private:

	Graphics & m_graphic;
};

#endif // !_VULKAN_DESCRIPTOR_H