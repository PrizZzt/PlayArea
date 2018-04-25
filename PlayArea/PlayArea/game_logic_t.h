#pragma once

#include <cstdint>

class map_t;
class player_t;
struct object_s;

class game_logic_t
{
public:
    virtual void init(map_t *_map) = 0;
    virtual void update(map_t *_map) = 0;
		virtual object_s *create_player_object(uint8_t _id, player_t *_player) = 0;
};

#include "map_t.h"
