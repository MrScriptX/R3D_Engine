#pragma once

#include "Common.h"

class Surface
{
public:
	Surface(Renderer::Vulkan& vulkan);
	~Surface();

private:
	Renderer::Vulkan& m_vulkan;
};

