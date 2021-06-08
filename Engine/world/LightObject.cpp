#include "../Includes/world/LightObject.h"

LightObject::LightObject()
{
	m_transform.position = { .0f, .0f, .0f };
	m_transform.rotation = { .0f, .0f, .0f };
}

LightObject::~LightObject()
{
}

Transform& LightObject::GetTransform()
{
	return m_transform;
}

void LightObject::SetTransform(Transform& transform)
{
	m_transform = transform;
}
