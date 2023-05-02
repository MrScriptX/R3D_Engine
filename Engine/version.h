#ifndef VRED_VERSION_H
#define VRED_VERSION_H

#include <cstdint>

#define ENGINE_VERSION_MAJOR 1
#define ENGINE_VERSION_MINOR 0
#define ENGINE_VERSION_PATCH 0

namespace vred
{
	constexpr uint32_t engine_version_major = ENGINE_VERSION_MAJOR;
	constexpr uint32_t engine_version_minor = ENGINE_VERSION_MINOR;
    constexpr uint32_t engine_version_patch = ENGINE_VERSION_PATCH;
}

#endif // !VRED_VERSION_H
