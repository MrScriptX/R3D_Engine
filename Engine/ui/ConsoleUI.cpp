#include "ConsoleUI.h"

ConsoleUI::ConsoleUI() : m_active(false), m_update(false), m_size(200.f, 400.f)
{
}

void ConsoleUI::Update()
{
	if (!m_active)
		return;

	ImGui::Begin("Console", &m_active);

	m_size = ImGui::GetWindowSize();
	ImGui::SetWindowSize(m_size);

	for (size_t i = 0; i < m_output.size(); i++)
	{
		ImGui::Text(m_output[i].c_str());
	}

	if (m_update)
	{
		ImGui::SetScrollHereY(1.f);
		m_update = false;
	}

	ImGui::End();
}

void ConsoleUI::SetActive(const bool active)
{
	m_active = active;
}

bool ConsoleUI::IsActive() const
{
	return m_active;
}
