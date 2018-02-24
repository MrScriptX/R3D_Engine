#pragma once

#include "ShaderLoader.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	void createPipeline();
	VkShaderModule createShaderModule(const std::vector<char>& code);
private:
	ShaderLoader m_shaderLoader;
};

