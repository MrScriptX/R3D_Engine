#pragma once

#include "VkHeader.h"

class Semaphore
{
public:
	Semaphore(VkDevice const& device);
	~Semaphore();

	VkSemaphore& getImageAvailable();
	VkSemaphore& getRenderFinished();

private:

	VkDevice const& m_device;

	VkSemaphore m_imageAvailable;
	VkSemaphore m_renderFinished;
};

