#ifndef R3DENGINE_WATCHER_H
#define R3DENGINE_WATCHER_H

#include <string>
#include <map>
#include <vector>
#include <glm/vec3.hpp>

#include "UI.h"

class Watcher : public UI
{
public:
	Watcher(const Watcher&) = delete;
	void operator=(const Watcher&) = delete;

	static Watcher& Get();
	void Update(const uint16_t width, const uint16_t height) override;

	static void WatchPosition(const std::string& name, const glm::vec3& pos);
	inline static void DropPosition(const std::string& name);

	static void WatchVariable(const std::string& name, const double& var);
	inline static void DropVariable(const std::string& name);

private:
	Watcher() = default;

	std::map<std::string, glm::vec3> m_positions;
	std::map<std::string, double> m_variables;
};

#endif
