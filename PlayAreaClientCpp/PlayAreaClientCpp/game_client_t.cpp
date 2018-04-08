#include <string>

#include "game_client_t.h"

#define MAX_LENGTH 41

game_client_t::game_client_t() :
	socket(io_context),
	resolver(io_context),
	map(nullptr)
{
}

game_client_t::~game_client_t()
{
	if (map)
		delete[] map;
}

void game_client_t::run(const char *_host, const int _port, const char *_login, const char *_password)
{
	boost::asio::connect(socket, resolver.resolve(_host, std::to_string(_port)));

	char request[MAX_LENGTH];
	request[0] = 6;
	for (int i = 0; i < strlen(_login) + 1; i++)request[i + 1] = _login[i];
	for (int i = 0; i < strlen(_password) + 1; i++)request[i + 21] = _password[i];
	boost::asio::write(socket, boost::asio::buffer(request, MAX_LENGTH));

	uint8_t credentials_answer[2];
	boost::asio::read(socket, boost::asio::buffer(credentials_answer, 1));
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
			if (boost::asio::read(socket, boost::asio::buffer(&server_action, 1)) == 0)continue;
		}
		catch (...)
		{
			continue;
		}
		switch (server_action)
		{
		case 1://MAP
			if (boost::asio::read(socket, boost::asio::buffer(&new_size_x, 1)) == 0)continue;
			if (boost::asio::read(socket, boost::asio::buffer(&new_size_y, 1)) == 0)continue;

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
					boost::asio::read(socket, boost::asio::buffer(object_info, 2));
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
	boost::asio::write(socket, boost::asio::buffer(&up, 1));
	boost::asio::read(socket, boost::asio::buffer(message, 2));
}

void game_client_t::down()
{
	char down = 2;
	boost::asio::write(socket, boost::asio::buffer(&down, 1));
	boost::asio::read(socket, boost::asio::buffer(message, 2));
}

void game_client_t::right()
{
	char right = 3;
	boost::asio::write(socket, boost::asio::buffer(&right, 1));
	boost::asio::read(socket, boost::asio::buffer(message, 2));
}

void game_client_t::left()
{
	char left = 4;
	boost::asio::write(socket, boost::asio::buffer(&left, 1));
	boost::asio::read(socket, boost::asio::buffer(message, 2));
}

void game_client_t::act()
{
	char act = 5;
	boost::asio::write(socket, boost::asio::buffer(&act, 1));
	boost::asio::read(socket, boost::asio::buffer(message, 2));
}
