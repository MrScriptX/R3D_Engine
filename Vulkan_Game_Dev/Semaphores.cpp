#include "Semaphores.h"



Semaphores::Semaphores(Renderer::Vulkan& vulkan) : m_vulkan(vulkan)
{
	VkSemaphoreCreateInfo sema_info = {};
	sema_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	sema_info.pNext = nullptr;
	sema_info.flags = 0;

	if (vkCreateSemaphore(m_vulkan.device, &sema_info, nullptr, &m_vulkan.image_available_semaphore) != VK_SUCCESS || 
		vkCreateSemaphore(m_vulkan.device, &sema_info, nullptr, &m_vulkan.finished_render_semaphore) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create semaphores!");
	}
}


Semaphores::~Semaphores()
{
}
