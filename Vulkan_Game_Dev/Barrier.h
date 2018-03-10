#pragma once

#include "VkHeader.h"
#include "CommandBuffer.h"


class Barrier
{
public:
	Barrier();
	~Barrier();

	static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkDevice& device, VkCommandPool& commandPool, VkQueue& graphicsQueue, bool stencil);

private:


};

