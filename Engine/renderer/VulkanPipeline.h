#ifndef _VULKAN_PIPELINE_H
#define _VULKAN_PIPELINE_H

#include <fstream>

#include "../graphics/Vertex.h"
#include "../graphics/Graphics.h"

enum class SHADER
{
	DEFAULT,
	NO_TEXTURE,
	TEXTURE
};

class VulkanPipeline
{
public:
	VulkanPipeline(Graphics & m_graphic);
	~VulkanPipeline();

	void createPipeline(Pipeline& pipeline);
	void bindPipeline(VkCommandBuffer& commandBuffer, std::shared_ptr<Pipeline> pipeline);
	void CreatePipelines();
	void DestroyPipelines();

	const Pipeline& GetPipeline(const SHADER shader);

private:

	std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	std::array<Pipeline, 3> m_pipelines;

	Graphics & m_graphic;
};

#endif _VULKAN_PIPELINE_H