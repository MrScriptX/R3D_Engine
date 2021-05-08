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

	void CreatePipelines();
	void DestroyPipelines();

	const Pipeline& GetPipeline(const SHADER shader);

private:
	void createPipeline(Pipeline& pipeline, const std::string& shader_file);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	std::vector<char> readFile(const std::string& filename);//should be in other file

	std::array<const std::string, 3> m_shader_files = {"assets/shaders/frag.spv", "assets/shaders/no_texture_shader.spv", "assets/shaders/texture_shader.spv"};
	std::array<Pipeline, 3> m_pipelines;
	Graphics & m_graphic;//should be shared ptr
};

#endif _VULKAN_PIPELINE_H