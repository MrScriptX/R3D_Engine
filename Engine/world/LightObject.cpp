#include "../Includes/world/LightObject.h"

LightObject::LightObject(TLIGHT type) : m_type(type)
{
	m_transform.position = { .0f, .0f, .0f };
	m_transform.direction = { .0f, .0f, .0f };
}

LightObject::~LightObject()
{
}

Light& LightObject::GetTransform()
{
	return m_transform;
}

const TLIGHT LightObject::GetType()
{
	return m_type;
}

void LightObject::SetTransform(Light& transform)
{
	m_transform = transform;
}
