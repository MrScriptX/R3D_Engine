#ifndef VRED_RENDERER_VSHADER_H
#define VRED_RENDERER_VSHADER_H

#include <string>
#include <vulkan/vulkan.h>
#include <vector>

#include <shaderc/shaderc.hpp>

namespace vred::renderer
{
	std::vector<char> read_shader_file(const std::string& shader_path);
	std::vector<uint32_t> compile_shader(const std::string& shader_path, const shaderc_shader_kind& stage, bool optimize = false);
	VkShaderModule create_shader_module(const uint32_t* code, size_t size, const VkDevice& device);
}

#endif // !VRED_RENDERER_VSHADER_H
