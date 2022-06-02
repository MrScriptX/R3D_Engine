#include "Watcher.h"

Watcher& Watcher::Get()
{
	static Watcher watcher;
	return watcher;
}

void Watcher::Update()
{
	if (!m_active)
		return;

	ImGui::Begin("Watcher", &m_active);

	ImGui::SetNextWindowSizeConstraints(ImVec2(200.f, 400.f), ImVec2(800, 800));

	ImGui::Text("Positon x:%f, y:%f, z:%f", m_position.x, m_position.y, m_position.z);

	ImGui::End();
}

void Watcher::SetPosition(const glm::vec3 pos)
{
	Watcher::Get().m_position = pos;
}
