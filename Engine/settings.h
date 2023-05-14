#ifndef VRED_SETTINGS_H
#define VRED_SETTINGS_H

#include <stdint.h>
#include <string>

namespace vred
{
	struct settings
	{
		uint32_t window_width = 1280;
		uint32_t window_height = 720;

		std::string app_name = "VRED Engine";
	};
}

#endif // !VRED_SETTINGS_H
