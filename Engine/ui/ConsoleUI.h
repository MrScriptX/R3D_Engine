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
	ConsoleUI(const ConsoleUI&) = delete;
	void operator=(const ConsoleUI&) = delete;

	static ConsoleUI& Get();
	void Update(const uint16_t width, const uint16_t height) override;

	template<typename ...Args>
	static void Log(const std::string msg, Args&&... args);

  private:
	ConsoleUI();

	bool m_update;
	
	std::vector<std::string> m_output;
};

template<typename ...Args>
static void ConsoleUI::Log(const std::string msg, Args&&... args)
{
	ConsoleUI* console = &ConsoleUI::Get();

	if (console->m_output.size() > 200)
		console->m_output.erase(console->m_output.begin());

	std::string parsed = std::vformat(msg, std::make_format_args(args...));
	console->m_output.push_back(parsed);

	console->m_update = true;
}

#endif
