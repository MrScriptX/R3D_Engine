#include "Semaphore.h"



Semaphore::Semaphore(VkDevice const& device) : m_device(device)
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_imageAvailable) != VK_SUCCESS ||
		vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_renderFinished) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create semaphore!");
	}
}


Semaphore::~Semaphore()
{
	vkDestroySemaphore(m_device, m_renderFinished, nullptr);
	vkDestroySemaphore(m_device, m_imageAvailable, nullptr);

	std::clog << "Semaphore destroyed\n";
}

VkSemaphore & Semaphore::getImageAvailable()
{
	return m_imageAvailable;
}

VkSemaphore & Semaphore::getRenderFinished()
{
	return m_renderFinished;
}
