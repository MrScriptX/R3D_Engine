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

private:
	Watcher() = default;

	std::map<std::string, glm::vec3> m_positions;
};

#endif
