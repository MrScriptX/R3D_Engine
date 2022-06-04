#ifndef LIGHT_MENU_H
#define LIGHT_MENU_H

#include <glm/vec3.hpp>
#include "ui/UI.h"

class LightMenu : public UI
{
public:
	LightMenu(glm::vec3& color, float& ambient, float& specular, float& diffuse);

	void Update(const uint16_t width, const uint16_t height) override;

private:
	glm::vec3& m_color;
	float& m_ambient;
	float& m_specular;
	float& m_diffuse;
};

#endif
