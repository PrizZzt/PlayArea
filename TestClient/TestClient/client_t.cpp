#include <string>

#include "player_points_info_u.h"
#include "client_t.h"

#define MAX_LENGTH 41

void client_t::recv_func()
{
	uint32_t new_length;

	uint8_t server_action;
	uint8_t result_code;
	uint8_t new_players_count;
	player_points_info_u player_points_info;
	while (is_recv_thread_running)
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
			if (boost::asio::read(socket, boost::asio::buffer(&size_x, 1)) == 0)continue;
			if (boost::asio::read(socket, boost::asio::buffer(&size_y, 1)) == 0)continue;

			new_length = size_x * size_y * 2;
			if (new_length != map_string_length)
			{
				if (map_string_length > 0)
					delete[] map_string;

				map_string_length = new_length;
				map_string = new uint8_t[map_string_length];
			}

			{
				std::unique_lock<std::mutex> lock(map_string_mutex);
				size_t reply_length = boost::asio::read(socket, boost::asio::buffer(map_string, map_string_length));
			}
			if (on_turn)on_turn();
			break;

		case 2://MESSAGE_OK
			if (boost::asio::read(socket, boost::asio::buffer(&result_code, 1)) == 0)continue;
			break;

		case 3://MESSAGE_FAIL
			if (boost::asio::read(socket, boost::asio::buffer(&result_code, 1)) == 0)continue;
			break;

		case 4://NAMES_LIST
			if (boost::asio::read(socket, boost::asio::buffer(&new_players_count, 1)) == 0)continue;
			check_players_count(new_players_count);
			for (uint8_t i = 0; i < players_count; i++)
			{
				uint8_t player_index;
				int8_t player_name_length;
				char player_name[20];
				if (boost::asio::read(socket, boost::asio::buffer(&player_index, 1)) == 0)break;
				if (boost::asio::read(socket, boost::asio::buffer(&player_name_length, 1)) == 0)break;
				if (boost::asio::read(socket, boost::asio::buffer(&player_name, player_name_length)) == 0)break;
				for (int i = 0; i < player_name_length; i++)
					player_names[player_index][i] = player_name[i];
				player_names[player_index][player_name_length] = 0;
			}
			break;

		case 5://POINTS_LIST
			if (boost::asio::read(socket, boost::asio::buffer(&new_players_count, 1)) == 0)continue;
			check_players_count(new_players_count);
			for (uint8_t i = 0; i < players_count; i++)
			{
				if (boost::asio::read(socket, boost::asio::buffer(&player_points_info.data, 5)) == 0)break;
				player_points[player_points_info.value.player_index] = player_points_info.value.player_points;
			}
			break;

		default:
			if (on_error)on_error();
			break;
		}
	}
}

client_t::client_t() :
    socket(io_context),
    resolver(io_context)
{
	player_names = nullptr;
}

client_t::~client_t()
{
	for (uint8_t i = 0; i < players_count; i++)
	{
		delete[] player_names[i];
	}
	delete[] player_points;

	if (socket.is_open())
		socket.close();

    is_recv_thread_running = false;
    recv_thread.join();

    delete[] map_string;
}

void client_t::run(const char *_host, const int _port, const char *_login, const char *_password)
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

	is_recv_thread_running = true;
	recv_thread = std::thread(&client_t::recv_func, this);
}

void client_t::up()
{
    char up = 1;
    boost::asio::write(socket, boost::asio::buffer(&up, 1));
}

void client_t::down()
{
    char down = 2;
    boost::asio::write(socket, boost::asio::buffer(&down, 1));
}

void client_t::right()
{
    char right = 3;
    boost::asio::write(socket, boost::asio::buffer(&right, 1));
}

void client_t::left()
{
    char left = 4;
    boost::asio::write(socket, boost::asio::buffer(&left, 1));
}

void client_t::act()
{
    char act = 5;
    boost::asio::write(socket, boost::asio::buffer(&act, 1));
}

void client_t::get_names()
{
	char get_names = 7;
	boost::asio::write(socket, boost::asio::buffer(&get_names, 1));
}

void client_t::get_scores()
{
	char get_scores = 8;
	boost::asio::write(socket, boost::asio::buffer(&get_scores, 1));
}
