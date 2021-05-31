#ifndef R3DENGINE_VULKAN_DESCRIPTOR_H_
#define R3DENGINE_VULKAN_DESCRIPTOR_H_

#include <array>
#include <stdexcept>

#include "../graphics/Graphics.h"
#include "../graphics/Vertex.h"

class VulkanDescriptor
{
  public:
	VulkanDescriptor(Graphics& graphic);
	~VulkanDescriptor();

	void createDescriptorPool();

	void createDescriptorSetLayout();

	void createDescriptorSet(const VkDescriptorPool& descriptor_pool, VkDescriptorSetLayout& layout, VkDescriptorSet& descriptor_set);
	void updateDescriptorSet();

	void bindDescriptorSet(VkCommandBuffer& commandBuffer, VkPipelineLayout& pipelineLayout, VkDescriptorSet& descriptorSet);

  private:
	Graphics& m_graphic;
};

#endif // !R3DENGINE_VULKAN_DESCRIPTOR_H
