#include "Texture.h"


#include <stb_image.h>


Texture::Texture(const std::string& texture_path) : m_texture_path(texture_path)
{
	m_texture_image = VK_NULL_HANDLE;
	m_texture_view = VK_NULL_HANDLE;
	m_texture_sampler = VK_NULL_HANDLE;
	m_texture_memory = VK_NULL_HANDLE;
	m_descriptor_set = VK_NULL_HANDLE;
}

Texture::~Texture()
{
}

void Texture::createTextureImage(std::shared_ptr<Renderer> renderer)
{
	int tex_width, tex_height, tex_channel;
	stbi_uc* pixels = stbi_load(m_texture_path.c_str(), &tex_width, &tex_height, &tex_channel, STBI_rgb_alpha);
	VkDeviceSize imageSize = tex_width * tex_height * 4;

	if (!pixels)
	{
		throw std::runtime_error("failed to load texture image!");
	}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	renderer->getBufferFactory()->createBuffer(stagingBuffer, stagingBufferMemory, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	void* data;
	vkMapMemory(renderer->getGraphic().device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(renderer->getGraphic().device, stagingBufferMemory);

	stbi_image_free(pixels);

	renderer->createImage(tex_width, tex_height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_texture_image, m_texture_memory);

	renderer->transitionImageLayout(m_texture_image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	renderer->copyBufferToImage(stagingBuffer, m_texture_image, static_cast<uint32_t>(tex_width), static_cast<uint32_t>(tex_height));

	renderer->transitionImageLayout(m_texture_image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(renderer->getGraphic().device, stagingBuffer, nullptr);
	vkFreeMemory(renderer->getGraphic().device, stagingBufferMemory, nullptr);
}

void Texture::createTextureImageView(std::shared_ptr<Renderer> renderer)
{
	m_texture_view = renderer->createImageView(m_texture_image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Texture::createTextureSampler(std::shared_ptr<Renderer> renderer)
{
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;

	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;

	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(renderer->getGraphic().device, &samplerInfo, nullptr, &m_texture_sampler) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture sampler!");
	}
}

const VkImageView& Texture::getImageView()
{
	return m_texture_view;
}

const VkSampler& Texture::getSampler()
{
	return m_texture_sampler;
}

VkDescriptorSet& Texture::getDescriptorSet()
{
	return m_descriptor_set;
}
