#ifndef VRED_RENDERER_VSHADER_H
#define VRED_RENDERER_VSHADER_H

#include <string>
#include <glslang/Public/ShaderLang.h>
#include <vulkan/vulkan.h>
#include <vector>

namespace vred::renderer
{
	std::vector<uint32_t> compile_shader(const std::string& shader_path, const EShLanguage& stage);
	VkShaderModule create_shader_module(const std::vector<uint32_t>& code, const VkDevice& device);
}

#endif // !VRED_RENDERER_VSHADER_H
