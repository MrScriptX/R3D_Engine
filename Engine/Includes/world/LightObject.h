#ifndef R3DENGINE_LIGHT_H_
#define R3DENGINE_LIGHT_H_

#include "../graphics/Vertex.h"

class LightObject
{
  public:
	LightObject();
	~LightObject();

	Transform& GetTransform();

	void SetTransform(Transform& transform);

  private:
	Transform m_transform;
};

#endif // !R3DENGINE_LIGHT_H_