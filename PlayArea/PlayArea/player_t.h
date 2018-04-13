#pragma once

#include <cstdint>

#include "object_s.h"
#include "player_list_s.h"

class map_t;

// ������� ������ ������
class player_t
{
	char    login[20];    // ����� ������
	char    password[20]; // ������ ������
	int32_t points;        // ���� ������

	player_t(const char *_login, const  char *_password);

public:
	object_s *object; // ��������������� ������ ������ �� �����, ������ ����

	player_t(const char *_login, const  char *_password, object_s *_object);
	player_t(const char *_login, const  char *_password, int32_t _points);
	~player_t();

	char *get_player_name() { return login; }
	int32_t get_points() { return points; }
	void set_object_action(client_action_e _action) { if (object)object->next_action = _action; }
	void add_score(int32_t _points, int32_t _min_points, int32_t _max_points)
	{
		if (points + _points < _min_points)
			points = _min_points;
		else
		{
			if (points > _max_points - _points)
				points = _max_points;
			else
				points += _points;
		}
	}

	bool check_credentials(char *_login, char *_password);

	void fill_player_info(player_list_s::player_info_s &_player_info)
	{
		_player_info.login = login;
		_player_info.password = password;
		_player_info.points = points;
	}
};
