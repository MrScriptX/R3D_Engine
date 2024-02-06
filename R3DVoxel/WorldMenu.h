#ifndef R3DVOXEL_WORLD_MENU_H
#define R3DVOXEL_WORLD_MENU_H

#include <ui/UI.h>

class WorldMenu : public UI
{
public:
	WorldMenu(uint8_t& load_radius);

	void Update(const uint16_t width, const uint16_t height) override;

private:
	uint8_t& m_load_radius;
};

#endif
