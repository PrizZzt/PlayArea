#pragma once

#include "../PlayArea/game_logic_t.h"

#include "bomberman_settings_s.h"
#include "bomberman_objects_e.h"

class bomberman_logic_t : public game_logic_t
{
	bomberman_settings_s settings;

	uint8_t get_bomb_state(uint8_t _player_state)
	{
		switch ((bomberman_objects_e)_player_state)
		{
		case bomberman_objects_e::PLAYER_WITH_BOMB_5:return (uint8_t)bomberman_objects_e::BOMB_5;
		case bomberman_objects_e::PLAYER_WITH_BOMB_4:return (uint8_t)bomberman_objects_e::BOMB_4;
		case bomberman_objects_e::PLAYER_WITH_BOMB_3:return (uint8_t)bomberman_objects_e::BOMB_3;
		case bomberman_objects_e::PLAYER_WITH_BOMB_2:return (uint8_t)bomberman_objects_e::BOMB_2;
		case bomberman_objects_e::PLAYER_WITH_BOMB_1:return (uint8_t)bomberman_objects_e::BOMB_1;
		default: throw std::exception("Wrong usage of get_bomb_state"); break;
		}
	}

	uint8_t next_bomb_state(uint8_t _object_state)
	{
		switch ((bomberman_objects_e)_object_state)
		{
		case bomberman_objects_e::PLAYER_WITH_BOMB_5:
			return (uint8_t)bomberman_objects_e::PLAYER_WITH_BOMB_4;
		case bomberman_objects_e::PLAYER_WITH_BOMB_4:
			return (uint8_t)bomberman_objects_e::PLAYER_WITH_BOMB_3;
		case bomberman_objects_e::PLAYER_WITH_BOMB_3:
			return (uint8_t)bomberman_objects_e::PLAYER_WITH_BOMB_2;
		case bomberman_objects_e::PLAYER_WITH_BOMB_2:
			return (uint8_t)bomberman_objects_e::PLAYER_WITH_BOMB_1;

		case bomberman_objects_e::PLAYER_WITH_BOMB_1:
		case bomberman_objects_e::BOMB_1:
			return (uint8_t)bomberman_objects_e::BOOM;

		case bomberman_objects_e::BOMB_5:
			return (uint8_t)bomberman_objects_e::BOMB_4;
		case bomberman_objects_e::BOMB_4:
			return (uint8_t)bomberman_objects_e::BOMB_3;
		case bomberman_objects_e::BOMB_3:
			return (uint8_t)bomberman_objects_e::BOMB_2;
		case bomberman_objects_e::BOMB_2:
			return (uint8_t)bomberman_objects_e::BOMB_1;

		default: return 0;
		}
	}

	int8_t get_meatchopper_value(object_s *_object)
	{
		if (_object == nullptr)
			return 0;

		switch ((bomberman_objects_e)_object->type)
		{
		case bomberman_objects_e::BOMB_1:              return -10;
		case bomberman_objects_e::BOMB_2:              return -8;
		case bomberman_objects_e::BOMB_3:              return -6;
		case bomberman_objects_e::BOMB_4:              return -4;
		case bomberman_objects_e::BOMB_5:              return -2;
		case bomberman_objects_e::PLAYER_WITH_BOMB_1:  return 1;
		case bomberman_objects_e::PLAYER_WITH_BOMB_2:  return 2;
		case bomberman_objects_e::PLAYER_WITH_BOMB_3:  return 4;
		case bomberman_objects_e::PLAYER_WITH_BOMB_4:  return 6;
		case bomberman_objects_e::PLAYER_WITH_BOMB_5:  return 8;
		case bomberman_objects_e::PLAYER:              return 10;

		case bomberman_objects_e::DESTRUCTIBLE_WALL:
		case bomberman_objects_e::UNDESTRUCTIBLE_WALL: return -1;

		default:                                       return 0;
		}
	}

	// ������ �� ������ ����� ����� ����������
	bool explode(map_t *_map, player_t *_killer, uint8_t _x, uint8_t _y)
	{
		object_s *object = _map->get_object(_x, _y);
		if (object)
		{
			switch ((bomberman_objects_e)object->type)
			{
			case bomberman_objects_e::PLAYER:
			case bomberman_objects_e::PLAYER_WITH_BOMB_1:
			case bomberman_objects_e::PLAYER_WITH_BOMB_2:
			case bomberman_objects_e::PLAYER_WITH_BOMB_3:
			case bomberman_objects_e::PLAYER_WITH_BOMB_4:
			case bomberman_objects_e::PLAYER_WITH_BOMB_5:
				if (object->player)
					object->player->add_score(-settings.player_dead_penalty, settings.player_score_min, settings.player_score_max);
			case bomberman_objects_e::DEAD_PLAYER:
				object->type = (uint8_t)bomberman_objects_e::DEAD_PLAYER;
				if (_killer && _killer->object != object)
					_killer->add_score(settings.player_score, settings.player_score_min, settings.player_score_max);
				return true;

			case bomberman_objects_e::MEATCHOPPER:
			case bomberman_objects_e::DEAD_MEATCHOPPER:
				object->type = (uint8_t)bomberman_objects_e::DEAD_MEATCHOPPER;
				if (_killer)
					_killer->add_score(settings.meatchopper_score, settings.player_score_min, settings.player_score_max);
				return true;

			case bomberman_objects_e::BOMB_1:
			case bomberman_objects_e::BOMB_2:
			case bomberman_objects_e::BOMB_3:
			case bomberman_objects_e::BOMB_4:
			case bomberman_objects_e::BOMB_5:
				object->type = (uint8_t)bomberman_objects_e::BOOM;
				object->to_delete = true;
				return true;

			case bomberman_objects_e::DESTRUCTIBLE_WALL:
			case bomberman_objects_e::DESTROYED_WALL:
				object->type = (uint8_t)bomberman_objects_e::DESTROYED_WALL;
				if (_killer)
					_killer->add_score(settings.wall_score, settings.player_score_min, settings.player_score_max);
				return true;

			case bomberman_objects_e::UNDESTRUCTIBLE_WALL:
				return true;
			}
		}
		else
			_map->set_object(new object_s(0, bomberman_objects_e::BOOM, true), _x, _y);
		return false;
	}

	void try_move_meatchopper(map_t *_map, uint8_t _x, uint8_t _y, int8_t _shift_x, int8_t _shift_y)
	{
		if (_x < -_shift_x || _y < -_shift_y || _x + _shift_x >= _map->get_size_x() || _y + _shift_y >= _map->get_size_y())
			return;

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
					target->type == (uint8_t)bomberman_objects_e::PLAYER ||
					target->type == (uint8_t)bomberman_objects_e::PLAYER_WITH_BOMB_1 ||
					target->type == (uint8_t)bomberman_objects_e::PLAYER_WITH_BOMB_2 ||
					target->type == (uint8_t)bomberman_objects_e::PLAYER_WITH_BOMB_3 ||
					target->type == (uint8_t)bomberman_objects_e::PLAYER_WITH_BOMB_4 ||
					target->type == (uint8_t)bomberman_objects_e::PLAYER_WITH_BOMB_5
					)
				{
					target->type = (uint8_t)bomberman_objects_e::DEAD_PLAYER;
					if (target->player)
						target->player->add_score(-settings.player_dead_penalty, settings.player_score_min, settings.player_score_max);
					return;
				}
			}
		}

		if (object && target == nullptr)
		{
			_map->set_object(nullptr, _x, _y);
			_map->set_object(object, _x + _shift_x, _y + _shift_y);
			return;
		}
	}

	void try_move_player(map_t *_map, uint8_t _x, uint8_t _y, int8_t _shift_x, int8_t _shift_y)
	{
		if (_x < -_shift_x || _y < -_shift_y || _x + _shift_x >= _map->get_size_x() || _y + _shift_y >= _map->get_size_y())
			return;

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

			if (object->type == (uint8_t)bomberman_objects_e::PLAYER)
				_map->set_object(nullptr, _x, _y);
			else
			{
				_map->set_object(new object_s(0, get_bomb_state(object->type), object->player), _x, _y);
				object->type = (uint8_t)bomberman_objects_e::PLAYER;
			}
			return;
		}
	}

public:
	void init(map_t *_map)override
	{
		settings.load("bomberman.settings.json");
		settings.save("bomberman.settings.json");

		for (uint8_t j = 0; j < _map->get_size_y(); j++)
		{
			for (uint8_t i = 0; i < _map->get_size_x(); i++)
			{
				if (i % 2 == 1 && j % 2 == 1)
					_map->set_object(new object_s(0, bomberman_objects_e::UNDESTRUCTIBLE_WALL, false), i, j);
			}
		}
		for (uint8_t i = 0; i < 40; i++)
		{
			object_s *wall = new object_s(i, bomberman_objects_e::DESTRUCTIBLE_WALL, false);
			if (_map->place_object_in_random_location(wall) == false)
			{
				delete wall;
				break;
			}
		}
		for (uint8_t i = 0; i < 4; i++)
		{
			object_s *meatchopper = new object_s(i, bomberman_objects_e::MEATCHOPPER, false);
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
		// ��������� ������������ �������, ��������� �������
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
						switch ((bomberman_objects_e)object->type)
						{
						case bomberman_objects_e::PLAYER:
						case bomberman_objects_e::PLAYER_WITH_BOMB_1:
						case bomberman_objects_e::PLAYER_WITH_BOMB_2:
						case bomberman_objects_e::PLAYER_WITH_BOMB_3:
						case bomberman_objects_e::PLAYER_WITH_BOMB_4:
						case bomberman_objects_e::PLAYER_WITH_BOMB_5:
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
								if ((bomberman_objects_e)object->type == bomberman_objects_e::PLAYER)
									object->type = (uint8_t)bomberman_objects_e::PLAYER_WITH_BOMB_5;
								break;
							}
							break;

						case bomberman_objects_e::DEAD_PLAYER:
							object->type = (uint8_t)bomberman_objects_e::PLAYER;

							if (_map->place_object_in_random_location(object))
								_map->set_object(nullptr, i, j);

							break;

						case bomberman_objects_e::DEAD_MEATCHOPPER:
							object->type = (uint8_t)bomberman_objects_e::MEATCHOPPER;

							if (_map->place_object_in_random_location(object))
								_map->set_object(nullptr, i, j);

							break;

						case bomberman_objects_e::DESTROYED_WALL:
							object->type = (uint8_t)bomberman_objects_e::DESTRUCTIBLE_WALL;

							if (_map->place_object_in_random_location(object))
								_map->set_object(nullptr, i, j);

							break;
						}
						object->next_action = client_action_e::NONE;
					}
				}
			}
		}
		// ��� �����
		object_s *target;
		for (uint8_t j = 0; j < _map->get_size_y(); j++)
		{
			for (uint8_t i = 0; i < _map->get_size_x(); i++)
			{
				if ((object = _map->get_object(i, j)) && object->type == (uint8_t)bomberman_objects_e::MEATCHOPPER && object->next_action == client_action_e::NONE)
				{
					// ���������� ������������ ���������, �� ������� ���� ������ ��������
					uint8_t range = i;
					if (j > range)range = j;
					if (_map->get_size_x() - i > range)range = _map->get_size_x() - i;
					if (_map->get_size_y() - j > range)range = _map->get_size_y() - j;

					int8_t values[4] = { 0,0,0,0 }; // ���� ����� � ����� ���� ������ �� ���� ��������
					uint8_t ranges[4] = { 0,0,0,0 }; // ��������� ������ �� ��� �������
					bool continue_check = true; // ���� ����������� ��������
					for (uint8_t k = 1; k <= range; k++)
					{
						// ���������� ��������, ���� �� ���� ������ �������� � �����������, ����� ��������� �����
						if (continue_check)
							continue_check = false;
						else
							break;

						if (values[0] == 0 && i >= k) // ������ ����� �� ������ � ���� ���� �� ��������
						{
							// ���������� �������� �����
							continue_check = true;
							values[0] = get_meatchopper_value(target = _map->get_object(i - k, j));
							ranges[0] = k;
						}
						if (values[1] == 0 && j >= k) // ������ ������ �� ������ � ���� ���� �� ��������
						{
							// ���������� �������� �����
							continue_check = true;
							values[1] = get_meatchopper_value(target = _map->get_object(i, j - k));
							ranges[1] = k;
						}
						if (values[2] == 0 && i + k < _map->get_size_x()) // ������ ������ �� ������ � ���� ���� �� ��������
						{
							// ���������� �������� ������
							continue_check = true;
							values[2] = get_meatchopper_value(target = _map->get_object(i + k, j));
							ranges[2] = k;
						}
						if (values[3] == 0 && j + k < _map->get_size_y()) // ������ ����� �� ������ � ���� ���� �� ��������
						{
							// ���������� �������� ����
							continue_check = true;
							values[3] = get_meatchopper_value(target = _map->get_object(i, j + k));
							ranges[3] = k;
						}
					}
					// ������� ������������ �������� ����� ���� ������� ��������
					uint8_t max_k = 0; // ����� ����������� � ������������ ����� � ����������� ���������� (������������� = 0)
					int8_t max_value = values[max_k]; // ���� ���������� �����������
					uint8_t min_range = ranges[max_k]; // ��������� ���������� �����������
					for (uint8_t k = 1; k < 4; k++)
					{
						if (values[k] > max_value || (values[k] == max_value && ranges[k] < min_range))
						{
							max_k = k;
							max_value = values[k];
							min_range = ranges[k];
						}
					}

					// �������� ���������� �������� ������������ ���� ������� ��������, ������� � ������ ���������
					for (uint8_t k = 0; k < 4; k++)
					{
						switch ((max_k + k) % 4)
						{
						case 0: if (values[0] >= 0)try_move_meatchopper(_map, i, j, -1, 0); else try_move_meatchopper(_map, i, j, 1, 0); break;
						case 1: if (values[1] >= 0)try_move_meatchopper(_map, i, j, 0, -1); else try_move_meatchopper(_map, i, j, 0, 1); break;
						case 2: if (values[2] >= 0)try_move_meatchopper(_map, i, j, 1, 0); else try_move_meatchopper(_map, i, j, -1, 0); break;
						case 3: if (values[3] >= 0)try_move_meatchopper(_map, i, j, 0, 1); else try_move_meatchopper(_map, i, j, 0, -1); break;
						}
					}
					object->next_action = client_action_e::ACT;
				}
			}
		}
		// ��������� ���� ����
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
						if (next_state == (uint8_t)bomberman_objects_e::BOOM)
						{
							if (object->type == (uint8_t)bomberman_objects_e::PLAYER_WITH_BOMB_1)
							{
								object->type = (uint8_t)bomberman_objects_e::DEAD_PLAYER;
								if (object->player)
									object->player->add_score(-settings.player_dead_penalty, settings.player_score_min, settings.player_score_max);
							}
							else
							{
								object->type = (uint8_t)bomberman_objects_e::BOOM;
								object->to_delete = true;
							}
							for (uint8_t k = 1; k < 4; k++)
							{
								if (i >= k)
								{
									if (explode(_map, object->player, i - k, j))break;
								}
							}
							for (uint8_t k = 1; k < 4; k++)
							{
								if (i + k < _map->get_size_x())
								{
									if (explode(_map, object->player, i + k, j))break;
								}
							}
							for (uint8_t k = 1; k < 4; k++)
							{
								if (j >= k)
								{
									if (explode(_map, object->player, i, j - k))break;
								}
							}
							for (uint8_t k = 1; k < 4; k++)
							{
								if (j + k < _map->get_size_y())
								{
									if (explode(_map, object->player, i, j + k))break;
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
		return new object_s(_id, bomberman_objects_e::PLAYER, _player);
	}
};
