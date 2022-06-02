#ifndef R3DENGINE_UI_H
#define R3DENGINE_UI_H

#include <imgui.h>

class UI
{
public:
	UI();

	virtual void Update() = 0;

	void SetActive(const bool active);
	bool IsActive() const;

protected:
	bool m_active;
	ImVec2 m_size;
};

#endif
