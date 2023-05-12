#ifndef R3DENGINE_VULKAN_PIPELINE_H_
#define R3DENGINE_VULKAN_PIPELINE_H_

#include <fstream>

#include "../graphics/Graphics.h"
#include "../graphics/Shaders.h"
#include "../graphics/Vertex.h"

#include "ihardware.h"
#include "iswapchain.h"
#include "render.h"

enum class TSHADER
{
	DEFAULT,
	NO_TEXTURE,
	TEXTURE,
	LIGHT_SOURCE
};

class VulkanPipeline
{
  public:
	VulkanPipeline(vred::renderer::ihardware& hw, vred::renderer::iswapchain& swapchain, vred::renderer::render& render_objects);
	~VulkanPipeline();

	void CreatePipelines();
	void DestroyPipelines();

	const Pipeline& GetPipeline(const TSHADER shader);

  private:
	void createPipeline(Pipeline& pipeline, const std::string& fragment_shader_file, const std::string& vertex_shader_file);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	std::vector<char> readFile(const std::string& filename); // should be in other file

	std::array<Pipeline, 4> m_pipelines;
	vred::renderer::ihardware& m_hw;
	vred::renderer::iswapchain& m_swapchain;
	vred::renderer::render& m_render_objects;
	Shaders m_shaders;
};

#endif // !R3DENGINE_VULKAN_PIPELINE_H_
