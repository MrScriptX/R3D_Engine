#include "WorldMenu.h"

WorldMenu::WorldMenu(uint8_t& load_radius) : m_load_radius(load_radius)
{
}

void WorldMenu::Update(const uint16_t width, const uint16_t height)
{
	ImGui::SetNextWindowSize(ImVec2(300.f, 100.f), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(width - ((width + 300.f) / 2), 20.f), ImGuiCond_Once);

	ImGui::Begin("World Menu", &m_active);

	ImGui::SliderInt("Load Radius", reinterpret_cast<int*>(&m_load_radius), 0, UINT8_MAX);

	ImGui::End();
}
