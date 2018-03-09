#pragma once

#include "VkHeader.h"

class TextureView
{
public:
	TextureView(VkDevice& device, VkImage& texture);
	~TextureView();

	static VkImageView createImageView(VkDevice& device, VkImage image, VkFormat format);

	VkImageView& getImageView();
private:

	VkDevice m_device;
	VkImageView m_imageView;
};

