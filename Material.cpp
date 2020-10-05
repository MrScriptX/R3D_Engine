#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::loadTexture(const std::string& texture_path)
{
	m_texture = std::make_shared<Texture>(texture_path);
}

std::shared_ptr<Texture> Material::getTexture()
{
	return m_texture;
}
