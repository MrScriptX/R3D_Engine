#pragma once

#include "VkHeader.h"
#include "Buffer.h"


class VkRenderer
{
public:
	VkRenderer(VkCommandBuffer & commandBuffer);
	~VkRenderer();

	void bindModel();
	void setViewport();
	void bindPipeline();
	void bindDescriptorSet();
	void draw();


private:
	 
	VkCommandBuffer & m_commandBuffer;
	Buffer m_buffer;
};

