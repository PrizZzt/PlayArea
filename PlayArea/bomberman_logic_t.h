#pragma once

#include "game_logic_t.h"

class bomberman_logic_t : public game_logic_t
{
	enum class objects_e : uint8_t
	{
		NONE,

		UNDESTRUCTIBLE_WALL,
		DESTRUCTIBLE_WALL,
		DESTROYED_WALL,

		PLAYER,
		PLAYER_WITH_BOMB_1,
		PLAYER_WITH_BOMB_2,
		PLAYER_WITH_BOMB_3,
		PLAYER_WITH_BOMB_4,
		PLAYER_WITH_BOMB_5,
		DEAD_PLAYER,

		MEAT_CHOPPER,
		DEAD_MEAT_CHOPPER,

		BOOM,
		BOMB_1,
		BOMB_2,
		BOMB_3,
		BOMB_4,
		BOMB_5
	};

	uint8_t get_bomb_state(uint8_t _player_state)
	{
		switch ((objects_e)_player_state)
		{
		case objects_e::PLAYER_WITH_BOMB_5:return (uint8_t)objects_e::BOMB_5;
		case objects_e::PLAYER_WITH_BOMB_4:return (uint8_t)objects_e::BOMB_4;
		case objects_e::PLAYER_WITH_BOMB_3:return (uint8_t)objects_e::BOMB_3;
		case objects_e::PLAYER_WITH_BOMB_2:return (uint8_t)objects_e::BOMB_2;
		case objects_e::PLAYER_WITH_BOMB_1:return (uint8_t)objects_e::BOMB_1;
		default: throw std::exception("Wrong usage of get_bomb_state"); break;
		}
	}

	uint8_t next_bomb_state(uint8_t _object_state)
	{
		switch ((objects_e)_object_state)
		{
		case objects_e::PLAYER_WITH_BOMB_5:
			return (uint8_t)objects_e::PLAYER_WITH_BOMB_4;
		case objects_e::PLAYER_WITH_BOMB_4:
			return (uint8_t)objects_e::PLAYER_WITH_BOMB_3;
		case objects_e::PLAYER_WITH_BOMB_3:
			return (uint8_t)objects_e::PLAYER_WITH_BOMB_2;
		case objects_e::PLAYER_WITH_BOMB_2:
			return (uint8_t)objects_e::PLAYER_WITH_BOMB_1;

		case objects_e::PLAYER_WITH_BOMB_1:
		case objects_e::BOMB_1:
			return (uint8_t)objects_e::BOOM;

		case objects_e::BOMB_5:
			return (uint8_t)objects_e::BOMB_4;
		case objects_e::BOMB_4:
			return (uint8_t)objects_e::BOMB_3;
		case objects_e::BOMB_3:
			return (uint8_t)objects_e::BOMB_2;
		case objects_e::BOMB_2:
			return (uint8_t)objects_e::BOMB_1;

		default: return 0;
		}
	}

public:
	void init(map_t *_map)override
	{
		for (uint8_t j = 0; j < _map->get_size_y(); j++)
		{
			for (uint8_t i = 0; i < _map->get_size_x(); i++)
			{
				if (i % 2 == 1 && j % 2 == 1)
					_map->set_object(new object_s((uint8_t)objects_e::UNDESTRUCTIBLE_WALL, false), i, j);
			}
		}
	}

	void update(map_t *_map)override
	{
		object_s *object;
		// Обработка передвижения игроков, исчезания вспышек
		for (uint8_t j = 0; j < _map->get_size_y(); j++)
		{
			for (uint8_t i = 0; i < _map->get_size_x(); i++)
			{
				object = _map->get_object(i, j);
				if (object)
				{
					if (object->to_delete)
					{
						delete object;
						_map->set_object(nullptr, i, j);
					}
					else
					{
						switch ((objects_e)object->type)
						{
						case objects_e::PLAYER:
						case objects_e::PLAYER_WITH_BOMB_1:
						case objects_e::PLAYER_WITH_BOMB_2:
						case objects_e::PLAYER_WITH_BOMB_3:
						case objects_e::PLAYER_WITH_BOMB_4:
						case objects_e::PLAYER_WITH_BOMB_5:
							switch (object->next_action)
							{
							case client_action_e::MOVE_UP:
								if (j > 0 && _map->has_no_object(i, j - 1))
								{
									_map->set_object(object, i, j - 1);
									if (object->type == (uint8_t)objects_e::PLAYER)
										_map->set_object(nullptr, i, j);
									else
									{
										_map->set_object(new object_s(get_bomb_state(object->type), object->player), i, j);
										object->type = (uint8_t)objects_e::PLAYER;
									}
								}
								break;

							case client_action_e::MOVE_DOWN:
								if (j < _map->get_size_y() - 1 && _map->has_no_object(i, j + 1))
								{
									_map->set_object(object, i, j + 1);
									if (object->type == (uint8_t)objects_e::PLAYER)
										_map->set_object(nullptr, i, j);
									else
									{
										_map->set_object(new object_s(get_bomb_state(object->type), object->player), i, j);
										object->type = (uint8_t)objects_e::PLAYER;
									}
								}
								break;

							case client_action_e::MOVE_RIGHT:
								if (i < _map->get_size_x() - 1 && _map->has_no_object(i + 1, j))
								{
									_map->set_object(object, i + 1, j);
									if (object->type == (uint8_t)objects_e::PLAYER)
										_map->set_object(nullptr, i, j);
									else
									{
										_map->set_object(new object_s(get_bomb_state(object->type), object->player), i, j);
										object->type = (uint8_t)objects_e::PLAYER;
									}
								}
								break;

							case client_action_e::MOVE_LEFT:
								if (i > 0 && _map->has_no_object(i - 1, j))
								{
									_map->set_object(object, i - 1, j);
									if (object->type == (uint8_t)objects_e::PLAYER)
										_map->set_object(nullptr, i, j);
									else
									{
										_map->set_object(new object_s(get_bomb_state(object->type), object->player), i, j);
										object->type = (uint8_t)objects_e::PLAYER;
									}
								}
								break;

							case client_action_e::ACT:
								if ((objects_e)object->type == bomberman_logic_t::objects_e::PLAYER)
									object->type = (uint8_t)objects_e::PLAYER_WITH_BOMB_5;
								break;
							}
							break;

						case objects_e::DEAD_PLAYER://TODO
							break;

						case objects_e::DEAD_MEAT_CHOPPER://TODO
							break;

						case objects_e::DESTROYED_WALL://TODO
							break;
						}
						object->next_action = client_action_e::NONE;
					}
				}
			}
		}
		// Обработка всех бомб
		for (uint8_t j = 0; j < _map->get_size_y(); j++)
		{
			for (uint8_t i = 0; i < _map->get_size_x(); i++)
			{
				object = _map->get_object(i, j);
				if (object)
				{
					uint8_t next_state = next_bomb_state(object->type);
					if (next_state != 0)
					{
						if (next_state == (uint8_t)objects_e::BOOM)
						{
							if (object->type == (uint8_t)objects_e::PLAYER_WITH_BOMB_1)
							{
								object->type = (uint8_t)objects_e::DEAD_PLAYER;
							}
							else
							{
								object->type = (uint8_t)objects_e::BOOM;
								object->to_delete = true;
							}
							for (uint8_t k = 1; k < 4; k++)
							{
								if (i - k >= 0)
								{
									if (explode(_map, i - k, j))break;
								}
							}
							for (uint8_t k = 1; k < 4; k++)
							{
								if (i + k < _map->get_size_x())
								{
									if (explode(_map, i + k, j))break;
								}
							}
							for (uint8_t k = 1; k < 4; k++)
							{
								if (j - k >= 0)
								{
									if (explode(_map, i, j - k))break;
								}
							}
							for (uint8_t k = 1; k < 4; k++)
							{
								if (j + k < _map->get_size_y())
								{
									if (explode(_map, i, j + k))break;
								}
							}
						}
						else
							object->type = next_state;
					}
				}
			}
		}
	}

	uint8_t get_player_type()override
	{
		return (uint8_t)objects_e::PLAYER;
	}

	bool explode(map_t *_map, uint8_t _x, uint8_t _y)
	{
		object_s *object = _map->get_object(_x, _y);
		if (object)
		{
			switch ((objects_e)object->type)
			{
			case objects_e::PLAYER:
			case objects_e::PLAYER_WITH_BOMB_1:
			case objects_e::PLAYER_WITH_BOMB_2:
			case objects_e::PLAYER_WITH_BOMB_3:
			case objects_e::PLAYER_WITH_BOMB_4:
			case objects_e::PLAYER_WITH_BOMB_5:
				object->type = (uint8_t)objects_e::DEAD_PLAYER;
				return true;
				break;

			case objects_e::MEAT_CHOPPER:
				object->type = (uint8_t)objects_e::DEAD_MEAT_CHOPPER;
				return true;
				break;

			case objects_e::BOMB_1:
			case objects_e::BOMB_2:
			case objects_e::BOMB_3:
			case objects_e::BOMB_4:
			case objects_e::BOMB_5:
				object->type = (uint8_t)objects_e::BOOM;
				object->to_delete = true;
				return true;
				break;

			case objects_e::DESTRUCTIBLE_WALL:
				object->type = (uint8_t)objects_e::DESTROYED_WALL;
				return true;
				break;

			case objects_e::UNDESTRUCTIBLE_WALL:
				return true;
				break;
			}
		}
		else
			_map->set_object(new object_s((uint8_t)objects_e::BOOM, true), _x, _y);
		return false;
	}
};
