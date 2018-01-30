#pragma once

#include "object_type_e.h"
#include "client_action_e.h"

// Представление любого объекта на карте
struct object_s
{
    object_type_e type;
    client_action_e      next_action;
    bool          to_delete;

    object_s(object_type_e _type)
    {
        type = _type;
        next_action = client_action_e::NONE;
        to_delete = false;
    }
};
