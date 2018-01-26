#pragma once

#include <cstdint>

#include "object_s.h"

class map_t
{
    object_s ***field;
    uint8_t size_x, size_y;

public:
    map_t(uint8_t _size_x, uint8_t _size_y)
    {
        size_x = _size_x;
        size_y = _size_y;

        field = new object_s**[size_y];
        for (uint8_t j = 0; j < size_y; j++)
        {
            field[j] = new object_s*[size_x];
            for (uint8_t i = 0; i < size_x; i++)
            {
                if (i % 2 == 1 && j % 2 == 1)
                    field[j][i] = new object_s(object_type_e::UNDESTRUCTIBLE_WALL);
                else
                    field[j][i] = nullptr;
            }
        }
    }

    ~map_t()
    {
        for (uint8_t j = 0; j < size_y; j++)
        {
            for (uint8_t i = 0; i < size_x; i++)
                delete field[j][i];
            delete[] field[j];
        }
    }

    uint8_t get_size_x() { return size_x; }
    uint8_t get_size_y() { return size_y; }
    uint32_t get_map_string_length() { return (size_x * size_y) + sizeof(size_x) + sizeof(size_y); }

    void fill_map_string(uint8_t *_target)
    {
        _target[0] = size_x;
        _target[1] = size_y;

        uint32_t position = 2;
        for (uint8_t j = 0; j < size_y; j++)
        {
            for (uint8_t i = 0; i < size_x; i++)
            {
                _target[position] = field[j][i] ? (uint8_t)field[j][i]->type : 0;
                position++;
            }
        }
    }

    void update()
    {
        for (uint8_t j = 0; j < size_y; j++)
        {
            for (uint8_t i = 0; i < size_x; i++)
            {
                if (field[j][i] && field[j][i]->to_delete)
                    field[j][i] = nullptr;
            }
        }
    }

    object_s *add_new_player_object()
    {
        for (uint8_t j = 0; j < size_y; j++)
        {
            for (uint8_t i = 0; i < size_x; i++)
            {
                if (field[j][i] == nullptr)
                {
                    field[j][i] = new object_s(object_type_e::PLAYER);
                    return field[j][i];
                }
            }
        }

        return nullptr;
    }
};