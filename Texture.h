#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "Renderer.h"

class Texture
{
public:
	Texture(const std::string& texture_path);
	~Texture();

	void createTextureImage(std::shared_ptr<Renderer> renderer);
	void createTextureImageView(std::shared_ptr<Renderer> renderer);
	void createTextureSampler(std::shared_ptr<Renderer> renderer);
	void AllocateDescriptorSet();

	const VkImageView& getImageView();
	const VkSampler& getSampler();

private:
	const std::string m_texture_path;

	VkImage m_texture_image;
	VkImageView m_texture_view;
	VkSampler m_texture_sampler;
	VkDeviceMemory m_texture_memory;

	VkDescriptorSet m_descriptor_set;
};

#endif // !_TEXTURE_H