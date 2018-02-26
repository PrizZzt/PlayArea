#pragma once

#include <cstdint>

#include "object_s.h"

class map_t;

// ������� ������ ������
class player_t
{
	char    login[20];    // ����� ������
	char    password[20]; // ������ ������
	int32_t score;        // ���� ������

public:
	object_s *object; // ��������������� ������ ������ �� �����, ������ ����

	player_t(char *_login, char *_password, object_s *_object);
	~player_t();

	char *get_player_name() { return login; }
	int32_t get_score() { return score; }
	void set_object_action(client_action_e _action) { if (object)object->next_action = _action; }
	void add_score(int32_t _score, int32_t _min_score, int32_t _max_score)
	{
		if (score + _score < _min_score)
			score = _min_score;
		else
		{
			if (score > _max_score - _score)
				score = _max_score;
			else
				score += _score;
		}
	}

	bool check_credentials(char *_login, char *_password);
};
