#include "vshader.h"

#include <fstream>
#include <iostream>

std::vector<char> read_file(const std::string& filename);

std::vector<char> vred::renderer::read_shader_file(const std::string& shader_path)
{
    std::ifstream file(shader_path, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("failed to open file : " + shader_path);

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return std::vector<char>(buffer.cbegin(), buffer.cend());
}

std::vector<uint32_t> vred::renderer::compile_shader(const std::string& shader_path, const shaderc_shader_kind& stage, bool optimize)
{
	shaderc::CompileOptions options;
	options.SetSourceLanguage(shaderc_source_language_glsl);
	// options.AddMacroDefinition("MY_DEFINE", "1");

	if (optimize) 
		options.SetOptimizationLevel(shaderc_optimization_level_size);

	auto file_buffer = read_file(shader_path);
	std::string source(file_buffer.begin(), file_buffer.end());

	shaderc::Compiler compiler;
	shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, stage, "file", options);

	if (module.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		std::cerr << module.GetErrorMessage();
		return std::vector<uint32_t>();
	}

	return std::vector<uint32_t>{ module.cbegin(), module.cend() };
}

VkShaderModule vred::renderer::create_shader_module(const uint32_t* code, size_t size, const VkDevice& device)
{
	VkShaderModuleCreateInfo shader_module_info = {};
	shader_module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shader_module_info.codeSize = size;
	shader_module_info.pCode = code;

	VkShaderModule shader_module;
	if (vkCreateShaderModule(device, &shader_module_info, nullptr, &shader_module) != VK_SUCCESS)
		throw std::runtime_error("failed to create shader module!");

	return shader_module;
}

std::vector<char> read_file(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("failed to open file : " + filename);

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}
