#include "../Includes/world/LightObject.h"

LightObject::LightObject(TLIGHT type) : m_type(type)
{
	m_properties.position = { .0f, .0f, .0f };
	m_properties.direction = { .0f, .0f, .0f };
}

LightObject::~LightObject()
{
}

void LightObject::SetPosition(glm::vec3 position)
{
	m_properties.position = position;
}

void LightObject::SetDirection(glm::vec3 direction)
{
	m_properties.direction = direction;
}

void LightObject::SetColor(glm::vec3 color)
{
	m_properties.color = color;
}

void LightObject::SetAmbientStrength(const float ambient)
{
	m_properties.ambient_strength = ambient;
}

void LightObject::SetDiffuseStrength(const float diffuse)
{
	m_properties.diffuse_strength = diffuse;
}

void LightObject::SetSpecularStrength(const float specular)
{
	m_properties.specular_strength = specular;
}

void LightObject::SetProperties(const LightProperties properties)
{
	m_properties = properties;
}

const LightProperties& LightObject::GetProperties()
{
	return m_properties;
}

const TLIGHT LightObject::GetType()
{
	return m_type;
}
