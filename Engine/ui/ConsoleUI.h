#ifndef R3DENGINE_CONSOLE_UI_H
#define R3DENGINE_CONSOLE_UI_H

#include <vector>
#include <string>
#include <imgui.h>

class ConsoleUI
{
public:
	void Update();
	void Log(const std::string& msg);

	void SetActive(const bool active);
	bool IsActive() const;

private:
	bool m_active;
	bool m_update;
	
	std::vector<std::string> m_output;
};

#endif
