#pragma once

#include <cstdint>

enum class server_action_e : uint8_t
{
    NONE = 0,

    MAP,
		MESSAGE_OK,
		MESSAGE_FAIL,

		NAMES_LIST,
		POINTS_LIST
};
