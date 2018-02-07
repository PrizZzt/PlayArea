#pragma once

#include <cstdint>

#include "object_s.h"
#include "server_action_e.h"
#include "game_logic_t.h"
#include "player_t.h"

class map_t
{
	game_logic_t *game_logic;
	object_s ***field;
	uint8_t size_x, size_y;

public:
	map_t(game_logic_t *_game_logic, uint8_t _size_x, uint8_t _size_y)
	{
		size_x = _size_x;
		size_y = _size_y;

		field = new object_s**[size_y];
		for (uint8_t j = 0; j < size_y; j++)
		{
			field[j] = new object_s*[size_x];
			for (uint8_t i = 0; i < size_x; i++)
				field[j][i] = nullptr;
		}

		game_logic = _game_logic;
		game_logic->init(this);
	}

	~map_t()
	{
		for (uint8_t j = 0; j < size_y; j++)
		{
			for (uint8_t i = 0; i < size_x; i++)
				delete field[j][i];
			delete[] field[j];
		}

		delete game_logic;
	}

	uint8_t get_size_x() { return size_x; }
	uint8_t get_size_y() { return size_y; }
	uint32_t get_map_string_length()
	{
		return
			sizeof(uint8_t)/* Äëÿ server_action_e */ +
			sizeof(size_x) +
			sizeof(size_y) +
			(size_x * size_y);
	}
	object_s *get_object(uint8_t _x, uint8_t _y) { return field[_y][_x]; }
	uint8_t get_player_type() { return game_logic->get_player_type(); }
	void set_object(object_s *_object, uint8_t _x, uint8_t _y) { field[_y][_x] = _object; }

	bool has_no_object(uint8_t _x, uint8_t _y)
	{
		if (field[_y][_x])
		{
			if (field[_y][_x]->to_delete)
			{
				delete field[_y][_x];
				field[_y][_x] = nullptr;

				return true;
			}
			return false;
		}
		return true;
	}

	void fill_map_string(uint8_t *_target)
	{
		_target[0] = (uint8_t)server_action_e::MAP;
		_target[1] = size_x;
		_target[2] = size_y;

		uint32_t position = 3;
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
				{
					if (field[j][i]->player && field[j][i]->player->object == field[j][i])
						field[j][i]->player->object = nullptr;
					delete field[j][i];
					field[j][i] = nullptr;
				}
			}
		}

		game_logic->update(this);
	}

	object_s *add_new_object(uint8_t _type, player_t *_player = nullptr)
	{
		for (uint8_t j = 0; j < size_y; j++)
		{
			for (uint8_t i = 0; i < size_x; i++)
			{
				if (field[j][i] == nullptr)
				{
					field[j][i] = new object_s(_type, _player);
					if (_player)
						_player->object = field[j][i];
					return field[j][i];
				}
			}
		}

		return nullptr;
	}
};
