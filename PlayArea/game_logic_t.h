#pragma once

class map_t;

class game_logic_t
{
public:
    virtual void init(map_t *_map) = 0;
    virtual void update(map_t *_map) = 0;
};

#include "map_t.h"
