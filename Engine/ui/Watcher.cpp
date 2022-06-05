#include "Watcher.h"

Watcher& Watcher::Get()
{
	static Watcher watcher;
	return watcher;
}

void Watcher::Update(const uint16_t width, const uint16_t height)
{
	if (!m_active)
		return;

	ImGui::SetNextWindowPos(ImVec2(width - 520.f, 20.f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(500.f, 500.f), ImGuiCond_Once);

	ImGui::Begin("Watcher", &m_active);

	ImGui::BeginTable("Position", 4, ImGuiTableFlags_BordersInner);

	ImGui::TableSetupColumn("Object");
	ImGui::TableSetupColumn("x coordinates", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize);
	ImGui::TableSetupColumn("y coordinates", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize);
	ImGui::TableSetupColumn("z coordinates", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize);

	ImGui::TableHeadersRow();

	for (auto i = m_positions.begin(); i != m_positions.end(); i++)
	{
		ImGui::TableNextRow();

		ImGui::TableNextColumn();
		ImGui::Text(i->first.c_str());

		ImGui::TableNextColumn();
		ImGui::Text("%f", i->second.x);

		ImGui::TableNextColumn();
		ImGui::Text("%f", i->second.y);

		ImGui::TableNextColumn();
		ImGui::Text("%f", i->second.z);
	}

	ImGui::EndTable();


	// Variable table
	ImGui::BeginTable("Variable", 2, ImGuiTableFlags_BordersInner);

	ImGui::TableSetupColumn("Name");
	ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_NoResize);

	ImGui::TableHeadersRow();

	for (auto i = m_variables.begin(); i != m_variables.end(); i++)
	{
		ImGui::TableNextRow();

		ImGui::TableNextColumn();
		ImGui::Text(i->first.c_str());

		ImGui::TableNextColumn();
		ImGui::Text("%f", i->second);
	}

	ImGui::EndTable();

	ImGui::End();
}

void Watcher::WatchPosition(const std::string& name, const glm::vec3& pos)
{
	auto it = Watcher::Get().m_positions.find(name);
	if (it != Watcher::Get().m_positions.end())
		it->second = pos;
	else
		Watcher::Get().m_positions.insert(std::pair<std::string, glm::vec3>(name, pos));
}

inline void Watcher::DropPosition(const std::string& name)
{
	Watcher::Get().m_positions.erase(name);	
}

void Watcher::WatchVariable(const std::string& name, const double& var)
{
	auto it = Watcher::Get().m_variables.find(name);
	if (it != Watcher::Get().m_variables.end())
		it->second = var;
	else
		Watcher::Get().m_variables.insert(std::pair<std::string, double>(name, var));
}

inline void Watcher::DropVariable(const std::string& name)
{
	Watcher::Get().m_variables.erase(name);
}
