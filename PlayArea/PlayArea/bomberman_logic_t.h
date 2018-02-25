#pragma once

#include "game_logic_t.h"

class bomberman_logic_t : public game_logic_t
{
public:
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

		MEATCHOPPER,
		DEAD_MEATCHOPPER,

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

	int8_t get_meatchopper_value(object_s *_object)
	{
		if (_object == nullptr)
			return 0;

		switch ((objects_e)_object->type)
		{
		case objects_e::BOMB_1:              return -10;
		case objects_e::BOMB_2:              return -8;
		case objects_e::BOMB_3:              return -6;
		case objects_e::BOMB_4:              return -4;
		case objects_e::BOMB_5:              return -2;
		case objects_e::PLAYER_WITH_BOMB_1:  return 1;
		case objects_e::PLAYER_WITH_BOMB_2:  return 2;
		case objects_e::PLAYER_WITH_BOMB_3:  return 4;
		case objects_e::PLAYER_WITH_BOMB_4:  return 6;
		case objects_e::PLAYER_WITH_BOMB_5:  return 8;
		case objects_e::PLAYER:              return 10;

		case objects_e::DESTRUCTIBLE_WALL:
		case objects_e::UNDESTRUCTIBLE_WALL: return -1;

		default:                             return 0;
		}
	}

	// Объект на данной точке карты взрывается
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

			case objects_e::MEATCHOPPER:
				object->type = (uint8_t)objects_e::DEAD_MEATCHOPPER;
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
			_map->set_object(new object_s(0, objects_e::BOOM, true), _x, _y);
		return false;
	}

	bool try_move_meatchopper(map_t *_map, uint8_t _x, uint8_t _y, int8_t _shift_x, int8_t _shift_y)
	{
		if (_x < -_shift_x || _y < -_shift_y || _x + _shift_x >= _map->get_size_x() || _y + _shift_y >= _map->get_size_y())
			return false;

		object_s *object = _map->get_object(_x, _y);

		object_s *target = _map->get_object(_x + _shift_x, _y + _shift_y);
		if (target)
		{
			if (target->to_delete)
			{
				delete target;
				target = nullptr;
				_map->set_object(nullptr, _x + _shift_x, _y + _shift_y);
			}
			else
			{
				if (
					target->type == (uint8_t)objects_e::PLAYER ||
					target->type == (uint8_t)objects_e::PLAYER_WITH_BOMB_1 ||
					target->type == (uint8_t)objects_e::PLAYER_WITH_BOMB_2 ||
					target->type == (uint8_t)objects_e::PLAYER_WITH_BOMB_3 ||
					target->type == (uint8_t)objects_e::PLAYER_WITH_BOMB_4 ||
					target->type == (uint8_t)objects_e::PLAYER_WITH_BOMB_5
					)
				{
					target->type = (uint8_t)objects_e::DEAD_PLAYER;
					return true;
				}
			}
		}

		if (object && target == nullptr)
		{
			_map->set_object(nullptr, _x, _y);
			_map->set_object(object, _x + _shift_x, _y + _shift_y);
			return true;
		}

		return false;
	}

	bool try_move_player(map_t *_map, uint8_t _x, uint8_t _y, int8_t _shift_x, int8_t _shift_y)
	{
		if (_x < -_shift_x || _y < -_shift_y || _x + _shift_x >= _map->get_size_x() || _y + _shift_y >= _map->get_size_y())
			return false;

		object_s *object = _map->get_object(_x, _y);

		object_s *target = _map->get_object(_x + _shift_x, _y + _shift_y);
		if (target && target->to_delete)
		{
			delete target;
			target = nullptr;
			_map->set_object(nullptr, _x + _shift_x, _y + _shift_y);
		}

		if (object && target == nullptr)
		{
			_map->set_object(object, _x + _shift_x, _y + _shift_y);

			if (object->type == (uint8_t)objects_e::PLAYER)
				_map->set_object(nullptr, _x, _y);
			else
			{
				_map->set_object(new object_s(0, get_bomb_state(object->type), object->player), _x, _y);
				object->type = (uint8_t)objects_e::PLAYER;
			}
			return true;
		}

		return false;
	}

public:
	void init(map_t *_map)override
	{
		for (uint8_t j = 0; j < _map->get_size_y(); j++)
		{
			for (uint8_t i = 0; i < _map->get_size_x(); i++)
			{
				if (i % 2 == 1 && j % 2 == 1)
					_map->set_object(new object_s(0, objects_e::UNDESTRUCTIBLE_WALL, false), i, j);
			}
		}
		for (uint8_t i = 0; i < 20; i++)
		{
			object_s *wall = new object_s(i, objects_e::DESTRUCTIBLE_WALL, false);
			if (_map->place_object_in_random_location(wall) == false)
			{
				delete wall;
				break;
			}
		}
		for (uint8_t i = 0; i < 4; i++)
		{
			object_s *meatchopper = new object_s(i, objects_e::MEATCHOPPER, false);
			if (_map->place_object_in_random_location(meatchopper) == false)
			{
				delete meatchopper;
				break;
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
								try_move_player(_map, i, j, 0, -1);
								break;

							case client_action_e::MOVE_DOWN:
								try_move_player(_map, i, j, 0, 1);
								break;

							case client_action_e::MOVE_RIGHT:
								try_move_player(_map, i, j, 1, 0);
								break;

							case client_action_e::MOVE_LEFT:
								try_move_player(_map, i, j, -1, 0);
								break;

							case client_action_e::ACT:
								if ((objects_e)object->type == bomberman_logic_t::objects_e::PLAYER)
									object->type = (uint8_t)objects_e::PLAYER_WITH_BOMB_5;
								break;
							}
							break;

						case objects_e::DEAD_PLAYER:
							object->type = (uint8_t)objects_e::PLAYER;

							if (_map->place_object_in_random_location(object))
								_map->set_object(nullptr, i, j);

							break;

						case objects_e::DEAD_MEATCHOPPER:
							object->type = (uint8_t)objects_e::MEATCHOPPER;

							if (_map->place_object_in_random_location(object))
								_map->set_object(nullptr, i, j);

							break;

						case objects_e::DESTROYED_WALL:
							object->type = (uint8_t)objects_e::DESTRUCTIBLE_WALL;

							if (_map->place_object_in_random_location(object))
								_map->set_object(nullptr, i, j);

							break;
						}
						object->next_action = client_action_e::NONE;
					}
				}
			}
		}
		// Ход ботов
		object_s *target;
		int8_t value;
		for (uint8_t j = 0; j < _map->get_size_y(); j++)
		{
			for (uint8_t i = 0; i < _map->get_size_x(); i++)
			{
				if ((object = _map->get_object(i, j)) && object->type == (uint8_t)objects_e::MEATCHOPPER && object->next_action == client_action_e::NONE)
				{
					uint8_t range = i;
					if (j > range)range = j;
					if (_map->get_size_x() - i > range)range = _map->get_size_x() - i;
					if (_map->get_size_y() - j > range)range = _map->get_size_y() - j;

					for (uint8_t k = 1; k <= range; k++)
					{
						if (i >= k && (value = get_meatchopper_value(target = _map->get_object(i - k, j))) != 0)
						{
							if (value > 0)
								try_move_meatchopper(_map, i, j, -1, 0);
							else
							{
								if (try_move_meatchopper(_map, i, j, 1, 0) || try_move_meatchopper(_map, i, j, 0, 1) || try_move_meatchopper(_map, i, j, 0, -1))
								{
								}
							}
							object->next_action = client_action_e::ACT;
							break;
						}
						if (j >= k && (value = get_meatchopper_value(target = _map->get_object(i, j - k))) != 0)
						{
							if (value > 0)
							{
								if (value > 1)
									try_move_meatchopper(_map, i, j, 0, -1);
							}
							else
							{
								if (try_move_meatchopper(_map, i, j, 1, 0) || try_move_meatchopper(_map, i, j, 0, 1) || try_move_meatchopper(_map, i, j, -1, 0))
								{
								}
							}
							object->next_action = client_action_e::ACT;
							break;
						}
						if (i + k < _map->get_size_x() && (value = get_meatchopper_value(target = _map->get_object(i + k, j))) != 0)
						{
							if (value > 0)
							{
								if (value > 1)
									try_move_meatchopper(_map, i, j, 1, 0);
							}
							else
							{
								if (try_move_meatchopper(_map, i, j, -1, 0) || try_move_meatchopper(_map, i, j, 0, 1) || try_move_meatchopper(_map, i, j, 0, -1))
								{
								}
							}
							object->next_action = client_action_e::ACT;
							break;
						}
						if (j + k < _map->get_size_y() && (value = get_meatchopper_value(target = _map->get_object(i, j + k))) != 0)
						{
							if (value > 0)
							{
								if (value > 1)
									try_move_meatchopper(_map, i, j, 0, 1);
							}
							else
							{
								if (try_move_meatchopper(_map, i, j, 1, 0) || try_move_meatchopper(_map, i, j, 0, -1) || try_move_meatchopper(_map, i, j, -1, 0))
								{
								}
							}
							object->next_action = client_action_e::ACT;
							break;
						}
					}
					if (try_move_meatchopper(_map, i, j, 1, 0) || try_move_meatchopper(_map, i, j, 0, -1) || try_move_meatchopper(_map, i, j, -1, 0) || try_move_meatchopper(_map, i, j, 0, 1))
					{
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
								if (i >= k)
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
								if (j >= k)
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

	object_s *create_player_object(uint8_t _id, player_t *_player)override
	{
		return new object_s(_id, objects_e::PLAYER, _player);
	}
};
