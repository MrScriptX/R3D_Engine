#ifndef _VULKAN_PIPELINE_H
#define _VULKAN_PIPELINE_H

#include <fstream>

#include "../graphics/Vertex.h"
#include "../graphics/Graphics.h"

enum class SHADER
{
	NONE,
	NO_TEXTURE,
	TEXTURE
};

class VulkanPipeline
{
public:
	VulkanPipeline(Graphics & m_graphic);
	~VulkanPipeline();

	void createPipeline(std::shared_ptr<Pipeline> pipeline);
	void bindPipeline(VkCommandBuffer& commandBuffer, std::shared_ptr<Pipeline> pipeline);
	void CreatePipelines();
	void DestroyPipelines();

	std::shared_ptr<Pipeline> GetPipeline(const SHADER shader);

private:

	std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	std::shared_ptr<Pipeline> m_texture_pipeline;
	std::shared_ptr<Pipeline> m_no_texture_pipeline;
	std::shared_ptr<Pipeline> m_base_pipeline;

	Graphics & m_graphic;
};

#endif _VULKAN_PIPELINE_H