#pragma once

#include "object_type_e.h"
#include "client_action_e.h"

class player_t;

// Представление любого объекта на карте
struct object_s
{
    object_type_e   type;
    client_action_e next_action;
    bool            to_delete;

    player_t *player;

    object_s(object_type_e _type, player_t *_player = nullptr)
    {
        type = _type;
        next_action = client_action_e::NONE;
        to_delete = false;

        player = _player;
    }
};
