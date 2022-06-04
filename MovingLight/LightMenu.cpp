#include "LightMenu.h"

LightMenu::LightMenu(glm::vec3& color, float& ambient, float& specular, float& diffuse) : m_color(color), m_ambient(ambient), m_specular(specular), m_diffuse(diffuse)
{
}

void LightMenu::Update(const uint16_t width, const uint16_t height)
{
	if (!m_active)
		return;

	ImGui::SetNextWindowPos(ImVec2(width - 320.f, 550.f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300.f, 300.f), ImGuiCond_Once);

	ImGui::Begin("Light Menu", &m_active);

	ImGui::SliderFloat("Color R", &m_color.x, 0.f, 1.0f);
	ImGui::SliderFloat("Color G", &m_color.y, 0.f, 1.0f);
	ImGui::SliderFloat("Color B", &m_color.z, 0.f, 1.0f);

	ImGui::SliderFloat("Ambient Strength", &m_ambient, 0.f, 5.f);
	ImGui::SliderFloat("Diffuse Strength", &m_diffuse, 0.f, 5.f);
	ImGui::SliderFloat("Specular Strength", &m_specular, 0.f, 5.f);

	ImGui::End();
}
