#pragma once

#include "VkHeader.h"

class Sampler
{
public:
	Sampler(VkDevice& device);
	~Sampler();

	VkSampler& getSampler();

private:

	VkDevice & m_device;
	VkSampler m_textureSampler;
};

