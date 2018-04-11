#include <memory>
#include <iostream>
#include <ctime>

#include "server_t.h"
#include "bomberman_logic_t.h"

void server_t::update_func()
{
	while (is_update_thread_running)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		map.update();
		map.fill_map_string(map_string);

		for (auto &session : sessions)
		{
			session->send_map(map_string, map_string_length);
		}

#pragma region TO DELETE
		HANDLE output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coord = { 0 };
		CONSOLE_SCREEN_BUFFER_INFO cBufferInfo;
		DWORD dwI;
		DWORD dwSize;

		if (output_handle == INVALID_HANDLE_VALUE) return;
		if (GetConsoleScreenBufferInfo(output_handle, &cBufferInfo))
		{
			dwSize = cBufferInfo.dwSize.X * cBufferInfo.dwSize.Y;
			FillConsoleOutputCharacter(output_handle, TEXT(' '), dwSize, coord, &dwI);
			SetConsoleCursorPosition(output_handle, coord);
		}

		time_t t = time(0);
		tm now;
		localtime_s(&now, &t);
		std::cout << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << std::endl;

		for (uint32_t i = 3; i < map.get_map_string_length(); i += 2)
		{
			switch ((bomberman_logic_t::objects_e)map_string[i])
			{
			case bomberman_logic_t::objects_e::NONE: std::cout << ' '; break;
			case bomberman_logic_t::objects_e::UNDESTRUCTIBLE_WALL:
			case bomberman_logic_t::objects_e::DESTRUCTIBLE_WALL: std::cout << '#'; break;
			case bomberman_logic_t::objects_e::DESTROYED_WALL: std::cout << '%'; break;
			case bomberman_logic_t::objects_e::PLAYER: std::cout << 'P'; break;
			case bomberman_logic_t::objects_e::PLAYER_WITH_BOMB_1: std::cout << '1'; break;
			case bomberman_logic_t::objects_e::PLAYER_WITH_BOMB_2: std::cout << '2'; break;
			case bomberman_logic_t::objects_e::PLAYER_WITH_BOMB_3: std::cout << '3'; break;
			case bomberman_logic_t::objects_e::PLAYER_WITH_BOMB_4: std::cout << '4'; break;
			case bomberman_logic_t::objects_e::PLAYER_WITH_BOMB_5: std::cout << '5'; break;
			case bomberman_logic_t::objects_e::DEAD_PLAYER: std::cout << 'X'; break;

			case bomberman_logic_t::objects_e::MEATCHOPPER: std::cout << 'M'; break;
			case bomberman_logic_t::objects_e::DEAD_MEATCHOPPER: std::cout << 'W'; break;

			case bomberman_logic_t::objects_e::BOOM: std::cout << '*'; break;
			case bomberman_logic_t::objects_e::BOMB_1: std::cout << '1'; break;
			case bomberman_logic_t::objects_e::BOMB_2: std::cout << '2'; break;
			case bomberman_logic_t::objects_e::BOMB_3: std::cout << '3'; break;
			case bomberman_logic_t::objects_e::BOMB_4: std::cout << '4'; break;
			case bomberman_logic_t::objects_e::BOMB_5: std::cout << '5'; break;

			default:
				std::cout << (char)map_string[i];
				break;
			}
			if (((i - 3) / 2 + 1) % map.get_size_x() == 0)std::cout << std::endl;
		}
#pragma endregion
	}
}

void server_t::do_accept()
{
	acceptor.async_accept([this](boost::system::error_code _ec, boost::asio::ip::tcp::socket _socket)
	{
		if (!_ec)
		{
			std::shared_ptr<session_t> new_session = std::make_shared<session_t>(std::move(_socket), this);
			sessions.push_front(new_session);
			new_session->start();
		}

		do_accept();
	});
}

server_t::server_t(game_logic_t *_game_logic, server_settings_s &_settings) :
	acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _settings.port)),
	map(_game_logic, _settings.map_settings)
{
	map_string_length = map.get_map_string_length();
	map_string = new uint8_t[map_string_length];

	players_count = 0;

	is_update_thread_running = true;
	update_thread = std::thread(&server_t::update_func, this);

	do_accept();
	io_context.run();
}

server_t::~server_t()
{
	is_update_thread_running = false;
	update_thread.join();

	delete[] map_string;
}

player_t *server_t::get_player(char *_login, char *_password)
{
	// ���� ����� ������������������
	for (uint8_t i = 0; i < players_count; i++)
	{
		if (players[i]->check_credentials(_login, _password))
		{
			// �����, ��������, ���� �� ��� � ����� ������ ������ �� ����� (�������� ���� ������ ��������� ����� �� ������ ��� � ����)
			if (players[i]->object == nullptr)
				players[i]->object = map.add_player_object(i, players[i]); // ��������� ������ ������
			return players[i]->object ? players[i] : nullptr; // ���������� �� ����� ������ ������� ������ ���� � ��� ���� ������ �� �����
		}
	}

	// ������������ ���������� �������
	if (players_count == MAX_PLAYERS_COUNT)return nullptr;

	// �������� ���������������� ������ ���� ������� ������� ��� ������
	object_s *new_object = map.add_player_object(players_count, nullptr);
	if (new_object)
	{
		// ������� - ��������� ������
		players[players_count] = new player_t(_login, _password, new_object);
		new_object->player = players[players_count];
		players_count++;

		return new_object->player;
	}
	else
		return nullptr; // �� ������� ������� ������ �� ����� - ������ ������ ���� �� ����� ���������
}
