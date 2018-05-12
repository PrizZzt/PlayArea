#pragma once

#include <functional>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "bomberman_objects_e.h"

class game_client_t
{
	SOCKET s;

	uint8_t message[2];

	uint8_t size_x, size_y;
	bomberman_objects_e **map;
	uint8_t player_id;
	uint8_t player_x, player_y;

	bool is_player(bomberman_objects_e obj)
	{
		return
			obj == bomberman_objects_e::PLAYER ||
			obj == bomberman_objects_e::PLAYER_WITH_BOMB_1 ||
			obj == bomberman_objects_e::PLAYER_WITH_BOMB_2 ||
			obj == bomberman_objects_e::PLAYER_WITH_BOMB_3 ||
			obj == bomberman_objects_e::PLAYER_WITH_BOMB_4 ||
			obj == bomberman_objects_e::PLAYER_WITH_BOMB_5 ||
			obj == bomberman_objects_e::DEAD_PLAYER;
	}

public:

	game_client_t();
	~game_client_t();

	void run(const char *_host, const int _port, const char *_login, const char *_password);

	bomberman_objects_e get_object(uint8_t _x, uint8_t _y) { return map[_y][_x]; }
	uint8_t             get_size_x() { return size_x; }
	uint8_t             get_size_y() { return size_y; }
	uint8_t             get_player_x() { return player_x; }
	uint8_t             get_player_y() { return player_y; }

	void up();
	void down();
	void right();
	void left();
	void act();

	std::function<void()> on_turn = nullptr;
	std::function<void()> on_error = nullptr;
};
