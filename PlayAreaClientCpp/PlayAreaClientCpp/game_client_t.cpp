#include <string>

#include "game_client_t.h"

#define MAX_LENGTH 41

game_client_t::game_client_t() :
	map(nullptr)
{
	WSADATA ws;
	WSAStartup(MAKEWORD(1, 1), &ws);
	s = socket(AF_INET, SOCK_STREAM, 0);
}

game_client_t::~game_client_t()
{
	closesocket(s);

	if (map)
		delete[] map;
}

void game_client_t::run(const char *_host, const int _port, const char *_login, const char *_password)
{
	sockaddr_in addr;

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, _host, &(addr.sin_addr));
	addr.sin_port = htons(_port);

	connect(s, (sockaddr*)&addr, sizeof(addr));

	char request[MAX_LENGTH];
	request[0] = 6;
	for (int i = 0; i < strlen(_login) + 1; i++)request[i + 1] = _login[i];
	for (int i = 0; i < strlen(_password) + 1; i++)request[i + 21] = _password[i];
	send(s, request, MAX_LENGTH, 0);

	uint8_t credentials_answer[2];
	recv(s, (char*)credentials_answer, 2, 0);
	if (credentials_answer[0] == 2)
		player_id = credentials_answer[1];
	else
		throw std::exception("Login was failed");

	uint8_t server_action;
	uint8_t new_size_x;
	uint8_t new_size_y;
	while (true)
	{
		try
		{
			if (recv(s, (char*)&server_action, 1, 0) == 0)continue;
		}
		catch (...)
		{
			continue;
		}
		switch (server_action)
		{
		case 1://MAP
			if (recv(s, (char*)&new_size_x, 1, 0) == 0)continue;
			if (recv(s, (char*)&new_size_y, 1, 0) == 0)continue;

			if (new_size_x != size_x || new_size_y != size_y)
			{
				if (map)
					delete[] map;

				size_x = new_size_x;
				size_y = new_size_y;

				map = new bomberman_objects_e*[size_y];
				for (uint8_t j = 0; j < size_y; j++)
				{
					map[j] = new bomberman_objects_e[size_x];
				}
			}

			uint8_t object_info[2];
			for (uint8_t j = 0; j < size_y; j++)
			{
				for (uint8_t i = 0; i < size_x; i++)
				{
					recv(s, (char*)object_info, 2, 0);
					map[j][i] = (bomberman_objects_e)object_info[0];
					if (is_player(map[j][i]) && object_info[1] == player_id)
					{
						player_x = i;
						player_y = j;
					}
				}
			}

			if (on_turn)on_turn();
			break;

		default:
			if (on_error)on_error();
			break;
		}
	}
}

void game_client_t::up()
{
	char up = 1;
	send(s, &up, 1, 0);
	recv(s, (char*)message, 2, 0);
}

void game_client_t::down()
{
	char down = 2;
	send(s, &down, 1, 0);
	recv(s, (char*)message, 2, 0);
}

void game_client_t::right()
{
	char right = 3;
	send(s, &right, 1, 0);
	recv(s, (char*)message, 2, 0);
}

void game_client_t::left()
{
	char left = 4;
	send(s, &left, 1, 0);
	recv(s, (char*)message, 2, 0);
}

void game_client_t::act()
{
	char act = 5;
	send(s, &act, 1, 0);
	recv(s, (char*)message, 2, 0);
}
