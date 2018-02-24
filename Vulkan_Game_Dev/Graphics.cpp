#include "Graphics.h"



Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}

void Graphics::createPipeline()
{
	auto vertShader = m_shaderLoader.loadFromFile("shader\vert.spv");
	auto fragShader = m_shaderLoader.loadFromFile("shader\frag.spv");
}
