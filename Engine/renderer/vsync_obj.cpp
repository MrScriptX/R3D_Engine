#include "vsync_obj.h"

#include <stdexcept>

VkFence vred::renderer::create_fence(const VkDevice& device)
{
	VkFenceCreateInfo fence_info = {};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.pNext = nullptr;
	fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkFence fence;
	if (vkCreateFence(device, &fence_info, nullptr, &fence) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create fence !");
	}

	return fence;
}

VkSemaphore vred::renderer::create_semaphore(const VkDevice& device)
{
	VkSemaphoreCreateInfo semaphore_info = {};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphore_info.pNext = nullptr;
	semaphore_info.flags = 0;

	VkSemaphore semaphore;
	if (vkCreateSemaphore(device, &semaphore_info, nullptr, &semaphore) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create semaphore !");
	}

	return semaphore;
}
