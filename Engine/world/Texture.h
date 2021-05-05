#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "../renderer/Renderer.h"

class Texture
{
public:
	Texture(const std::string& texture_path, std::shared_ptr<Renderer> p_renderer);
	~Texture();

	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();

	const VkImageView& getImageView();
	const VkSampler& getSampler();
	VkDescriptorSet& getDescriptorSet();

private:
	const std::string m_texture_path;

	VkImage m_texture_image;
	VkImageView m_texture_view;
	VkSampler m_texture_sampler;
	VkDeviceMemory m_texture_memory;

	VkDescriptorSet m_descriptor_set;

	std::shared_ptr<Renderer> mp_renderer;
};

#endif // !_TEXTURE_H