#ifndef VRED_RENDERER_VDEVICES_H
#define VRED_RENDERER_VDEVICES_H

#include <vulkan/vulkan.h>

#include "interface.h"

namespace vred::renderer
{
	VkPhysicalDevice choose_device(const interface& o);
}

#endif // !VRED_RENDERER_VDEVICES_H
