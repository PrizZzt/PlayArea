#pragma once

#include "object_type_e.h"
#include "action_e.h"

// Представление любого объекта на карте
struct object_s
{
    object_type_e type;
    action_e      next_action;
    bool          to_delete;

    object_s(object_type_e _type)
    {
        type = _type;
        next_action = action_e::NONE;
        to_delete = false;
    }
};
