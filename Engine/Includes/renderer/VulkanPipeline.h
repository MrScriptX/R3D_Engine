#ifndef _VULKAN_PIPELINE_H
#define _VULKAN_PIPELINE_H

#include <fstream>

#include "../graphics/Vertex.h"
#include "../graphics/Graphics.h"
#include "../graphics/Shaders.h"

enum class TSHADER
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

	const Pipeline& GetPipeline(const TSHADER shader);

private:
	void createPipeline(Pipeline& pipeline, const std::string& fragment_shader_file, const std::string& vertex_shader_file);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	std::vector<char> readFile(const std::string& filename);//should be in other file

	std::array<Pipeline, 3> m_pipelines;
	Graphics & m_graphic;//should be shared ptr
	Shaders m_shaders;
};

#endif _VULKAN_PIPELINE_H