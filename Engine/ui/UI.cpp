#include "UI.h"

UI::UI() : m_active(false), m_size(200.f, 400.f)
{
}

void UI::SetActive(const bool active)
{
	m_active = active;
}

bool UI::IsActive() const
{
	return m_active;
}
