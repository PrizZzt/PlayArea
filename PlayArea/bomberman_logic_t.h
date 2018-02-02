#pragma once

#include "game_logic_t.h"

class bomberman_logic_t : public game_logic_t
{
public:
    void init(map_t *_map)override
    {
        for (uint8_t j = 0; j < _map->get_size_y(); j++)
        {
            for (uint8_t i = 0; i < _map->get_size_x(); i++)
            {
                if (i % 2 == 1 && j % 2 == 1)
                    _map->set_object(new object_s(object_type_e::UNDESTRUCTIBLE_WALL), i, j);
            }
        }
    }

    void update(map_t *_map)override
    {
        for (uint8_t j = 0; j < _map->get_size_y(); j++)
        {
            for (uint8_t i = 0; i < _map->get_size_x(); i++)
            {
                object_s *object = _map->get_object(i, j);
                if (object)
                {
                    if (object->to_delete)
                    {
                        delete object;
                        _map->set_object(nullptr, i, j);
                    }
                    else
                    {
                        switch (object->type)
                        {
                        case object_type_e::PLAYER:
                            switch (object->next_action)
                            {
                            case client_action_e::MOVE_UP:
                                if (j > 0 && _map->has_no_object(i, j - 1))
                                {
                                    _map->set_object(object, i, j - 1);
                                    _map->set_object(nullptr, i, j);
                                }
                                break;

                            case client_action_e::MOVE_DOWN:
                                if (j < _map->get_size_y() - 1 && _map->has_no_object(i, j + 1))
                                {
                                    _map->set_object(object, i, j + 1);
                                    _map->set_object(nullptr, i, j);
                                }
                                break;

                            case client_action_e::MOVE_RIGHT:
                                if (i < _map->get_size_x() - 1 && _map->has_no_object(i + 1, j))
                                {
                                    _map->set_object(object, i + 1, j);
                                    _map->set_object(nullptr, i, j);
                                }
                                break;

                            case client_action_e::MOVE_LEFT:
                                if (i > 0 && _map->has_no_object(i - 1, j))
                                {
                                    _map->set_object(object, i - 1, j);
                                    _map->set_object(nullptr, i, j);
                                }
                                break;

                            case client_action_e::ACT:
                                break;
                            }
                            break;
                        }
                        object->next_action = client_action_e::NONE;
                    }
                }
            }
        }
    }
};