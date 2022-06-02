#ifndef R3DENGINE_WATCHER_H
#define R3DENGINE_WATCHER_H

#include <string>
#include <glm/vec3.hpp>

#include "UI.h"

class Watcher : public UI
{
public:
	Watcher(const Watcher&) = delete;
	void operator=(const Watcher&) = delete;

	static Watcher& Get();
	void Update() override;

	static void SetPosition(const glm::vec3 pos);

private:
	Watcher() = default;

	glm::vec3 m_position = glm::vec3();
};

#endif
