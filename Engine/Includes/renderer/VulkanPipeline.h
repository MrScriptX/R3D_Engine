#ifndef R3DENGINE_VULKAN_PIPELINE_H_
#define R3DENGINE_VULKAN_PIPELINE_H_

#include <fstream>

#include "../graphics/Graphics.h"
#include "../graphics/Shaders.h"
#include "../graphics/Vertex.h"

enum class TSHADER
{
	DEFAULT,
	NO_TEXTURE,
	TEXTURE
};

class VulkanPipeline
{
  public:
	VulkanPipeline(Graphics& m_graphic);
	~VulkanPipeline();

	void CreatePipelines();
	void DestroyPipelines();

	const Pipeline& GetPipeline(const TSHADER shader);

  private:
	void createPipeline(Pipeline& pipeline, const std::string& fragment_shader_file, const std::string& vertex_shader_file);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	std::vector<char> readFile(const std::string& filename); // should be in other file

	std::array<Pipeline, 3> m_pipelines;
	Graphics& m_graphic; // should be shared ptr
	Shaders m_shaders;
};

#endif // !R3DENGINE_VULKAN_PIPELINE_H_
