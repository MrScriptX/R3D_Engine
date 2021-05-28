#include "../Includes/world/LightObject.h"

LightObject::LightObject()
{
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
