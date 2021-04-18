#ifndef _VULKAN_PIPELINE_H
#define _VULKAN_PIPELINE_H

#include <fstream>

#include "../Vertex.h"
#include "../Graphics.h"

class VulkanPipeline
{
public:
	VulkanPipeline(Graphics & m_graphic);
	~VulkanPipeline();

	void createPipeline(Pipeline& pipeline);
	void bindPipeline(VkCommandBuffer& commandBuffer, Pipeline& pipeline);

private:

	std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	Graphics & m_graphic;
};

#endif _VULKAN_PIPELINE_H