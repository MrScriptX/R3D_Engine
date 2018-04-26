#pragma once

#include "Common.h"

class Semaphores
{
public:
	Semaphores(Renderer::Vulkan& vulkan);
	~Semaphores();

private:

	Renderer::Vulkan& m_vulkan;
};

