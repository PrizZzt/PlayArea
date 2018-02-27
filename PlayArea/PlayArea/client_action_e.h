#pragma once

#include <cstdint>

enum class client_action_e : uint8_t
{
    NONE = 0,

    MOVE_UP,
    MOVE_DOWN,
    MOVE_RIGHT,
    MOVE_LEFT,

    ACT,

    CREDENTIALS,

		GET_NAMES,
		GET_POINTS
};
