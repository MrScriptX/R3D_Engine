#pragma once

#include "Common.h"

class Window
{
public:
	Window(Renderer::Vulkan& vulkan);
	~Window();

private:

	Renderer::Vulkan& m_vulkan;
};

