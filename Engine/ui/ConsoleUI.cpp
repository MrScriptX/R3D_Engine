#include "ConsoleUI.h"

ConsoleUI::ConsoleUI() : UI(), m_update(false)
{
}

ConsoleUI& ConsoleUI::Get()
{
	static ConsoleUI console;
	return console;
}

void ConsoleUI::Update(const uint16_t width, const uint16_t height)
{
	if (!m_active)
		return;

	ImGui::SetNextWindowPos(ImVec2(20.f, 20.f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500.f, 1000.f), ImGuiCond_Once);

	ImGui::Begin("Console", &m_active);

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
