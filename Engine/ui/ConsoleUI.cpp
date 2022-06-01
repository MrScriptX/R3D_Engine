#include "../Includes/ui/ConsoleUI.h"

void ConsoleUI::Update()
{
	if (!m_active)
		return;

	ImGui::Begin("Console", &m_active);

	ImVec2 size(200, 400);
	ImGui::SetWindowSize(size);

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

void ConsoleUI::Log(const std::string& msg)
{
	if (m_output.size() > 200)
		m_output.erase(m_output.begin());
	else
		m_output.push_back(msg);

	m_update = true;
}

void ConsoleUI::SetActive(const bool active)
{
	m_active = active;
}

bool ConsoleUI::IsActive() const
{
	return m_active;
}
