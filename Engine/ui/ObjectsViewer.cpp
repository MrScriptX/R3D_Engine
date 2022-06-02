#include "ObjectsViewer.h"

ObjectsViewer::ObjectsViewer() : m_active(false), m_size(400.f, 400.f)
{
}

void ObjectsViewer::Update()
{
	if (!m_active)
		return;

	ImGui::Begin("Objects Viewer", &m_active);

	m_size = ImGui::GetWindowSize();
	ImGui::SetWindowSize(m_size);

	ImGui::End();
}

bool ObjectsViewer::IsActive() const
{
	return m_active;
}

void ObjectsViewer::SetActive(const bool active)
{
	m_active = active;
}
