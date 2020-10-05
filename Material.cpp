#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::loadTexture(const std::string& texture_path, std::shared_ptr<Renderer> renderer)
{
	m_texture = std::make_shared<Texture>(texture_path);
	m_texture->createTextureImage(renderer);
	m_texture->createTextureImageView(renderer);
	m_texture->createTextureSampler(renderer);
}

std::shared_ptr<Texture> Material::getTexture()
{
	return m_texture;
}
