#include "VkRenderer.h"



VkRenderer::VkRenderer(VkCommandBuffer & commandBuffer) : m_commandBuffer(commandBuffer)
{
}


VkRenderer::~VkRenderer()
{
}

void VkRenderer::bindModel()
{
	m_buffer.bindVertexBuffer();
	m_buffer.bindIndexBuffer();
}

void VkRenderer::setViewport()
{
}

void VkRenderer::bindPipeline()
{
}

void VkRenderer::bindDescriptorSet()
{
}

void VkRenderer::draw()
{
}
