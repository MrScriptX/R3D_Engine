#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
	m_texture.reset();
}

void Material::loadTexture(const std::string& texture_path, std::shared_ptr<Renderer> renderer)
{
	m_texture = std::make_shared<Texture>(texture_path, renderer);
	m_texture->createTextureImage();
	m_texture->createTextureImageView();
	m_texture->createTextureSampler();
}

std::shared_ptr<Texture> Material::getTexture()
{
	return m_texture;
}

void Material::destroyTexture()
{
	m_texture.reset();
	m_texture = nullptr;
}
