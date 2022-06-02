#ifndef R3DENGINE_OBJECTS_VIEWER_H
#define R3DENGINE_OBJECTS_VIEWER_H

#include <imgui.h>

class ObjectsViewer
{
public:
	ObjectsViewer();

	void Update();

	bool IsActive() const;
	void SetActive(const bool active);

 private:
	bool m_active;
	ImVec2 m_size;
};

#endif