#ifndef R3DENGINE_UI_H
#define R3DENGINE_UI_H

#include <memory>
#include <imgui.h>

class UI
{
public:
	UI();

	virtual void Update(const uint16_t width, const uint16_t height) = 0;

	void SetActive(const bool active);
	bool IsActive() const;

protected:
	bool m_active;
	ImVec2 m_size;
};

#endif
