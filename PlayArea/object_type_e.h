#pragma once

#include <cstdint>

enum class object_type_e : uint8_t
{
    NONE = 0,

    PLAYER,

    UNDESTRUCTIBLE_WALL,
    DESTRUCTIBLE_WALL
};
