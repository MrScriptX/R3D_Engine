#include "ConsoleUI.h"

ConsoleUI::ConsoleUI() : m_update(false)
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
