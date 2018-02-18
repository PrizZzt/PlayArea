#pragma once

#include <cstdint>

enum class server_action_e : uint8_t
{
    NONE = 0,

    MAP,
    RESULT
};
