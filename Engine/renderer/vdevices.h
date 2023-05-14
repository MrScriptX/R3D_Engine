#ifndef VRED_RENDERER_VDEVICES_H
#define VRED_RENDERER_VDEVICES_H

#include <vulkan/vulkan.h>

#include "ihardware.h"

namespace vred::renderer
{
	VkPhysicalDevice choose_device(const ihardware& hw);
	void create_device_interface(ihardware& hw);
}

#endif // !VRED_RENDERER_VDEVICES_H
