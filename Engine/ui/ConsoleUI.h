#ifndef R3DENGINE_CONSOLE_UI_H
#define R3DENGINE_CONSOLE_UI_H

#include <vector>
#include <string>
#include <format>
#include <imgui.h>

#include "UI.h"

class ConsoleUI : public UI
{
public:
	ConsoleUI();

	void Update() override;

	template<typename ...Args>
	void Log(std::string msg, Args&&... args);

private:
	bool m_update;
	
	std::vector<std::string> m_output;
};

template<typename ...Args>
void ConsoleUI::Log(std::string msg, Args&&... args)
{
	if (m_output.size() > 200)
		m_output.erase(m_output.begin());

	std::string parsed = std::format(msg, std::forward<Args>(args)...);
	m_output.push_back(parsed);

	m_update = true;
}

#endif
