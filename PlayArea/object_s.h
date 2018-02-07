#pragma once

#include "client_action_e.h"

class player_t;

// Представление любого объекта на карте
struct object_s
{
	uint8_t         type;
	client_action_e next_action;
	bool            to_delete;

	player_t *player;

	object_s(uint8_t _type, player_t *_player = nullptr)
	{
		type = _type;
		next_action = client_action_e::NONE;
		to_delete = false;

		player = _player;
	}

	object_s(uint8_t _type, bool _to_delete = false)
	{
		type = _type;
		next_action = client_action_e::NONE;
		to_delete = _to_delete;

		player = nullptr;
	}
};
