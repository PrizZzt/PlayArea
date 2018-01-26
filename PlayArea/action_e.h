#pragma once

#include <cstdint>

enum class action_e : uint8_t
{
    NONE = 0,

    MOVE_UP,
    MOVE_DOWN,
    MOVE_RIGHT,
    MOVE_LEFT,

    ACT,

    CREDENTIALS
};
