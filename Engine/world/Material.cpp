#include "Material.h"

Material::Material(const TSHADER shader, std::shared_ptr<Renderer> p_renderer) : m_shader(shader), mp_renderer(p_renderer)
{
	m_descriptor_set = VK_NULL_HANDLE;
}

Material::~Material()
{
	m_texture.reset();
}

void Material::setColor(glm::vec3 color)
{
	m_color = std::make_shared<glm::vec3>(color);
}

void Material::clearColor()
{
	m_color.reset();
	m_color = nullptr;
}

void Material::loadTexture(const std::string& texture_path, std::shared_ptr<Renderer> renderer)
{
	m_texture = std::make_shared<Texture>(texture_path, renderer);
	m_texture->createTextureImage();
	m_texture->createTextureImageView();
	m_texture->createTextureSampler();
}

void Material::destroyTexture()
{
	m_texture.reset();
	m_texture = nullptr;
}

std::shared_ptr<glm::vec3> Material::getColor()
{
	return m_color;
}

std::shared_ptr<Texture> Material::getTexture()
{
	return m_texture;
}

const Pipeline& Material::GetPipeline()
{
	return mp_renderer->GetPipelineFactory()->GetPipeline(m_shader);
}

VkDescriptorSet& Material::getDescriptorSet()
{
	return m_descriptor_set;
}
