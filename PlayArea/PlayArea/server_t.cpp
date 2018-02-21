#include <memory>
#include <iostream>
#include <ctime>

#include "server_t.h"

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
			switch (map_string[i])
			{
			case 0:
				std::cout << ' ';
				break;

			case 1:
				std::cout << '*';
				break;

			case 2:
			case 3:
				std::cout << '#';
				break;

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

server_t::server_t(game_logic_t *_game_logic, short _port) :
	acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _port)),
	map(_game_logic, 20, 20)
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
	// Ищем среди зарегистрированных
	for (uint16_t i = 0; i < players_count; i++)
	{
		if (players[i]->check_credentials(_login, _password))
		{
			// Нашли, проверим, есть ли уже у этого игрока объект на карте (возможно если пришли повторные креды на игрока уже в игре)
			if (players[i]->object == nullptr)
				players[i]->object = map.add_player_object(i, players[i]); // Добавляем объект игроку
			return players[i]->object ? players[i] : nullptr; // Возвращаем из этого метода игроков только если у них есть объект на карте
		}
	}

	// Максимальное количество игроков
	if (players_count == UINT8_MAX)return nullptr;

	// Пытаемся зарегистрировать игрока если удастся создать его объект
	object_s *new_object = map.add_player_object(players_count, nullptr);
	if (new_object)
	{
		// Удалось - добавляем учетку
		players[players_count] = new player_t(_login, _password, new_object);
		new_object->player = players[players_count];
		players_count++;

		return new_object->player;
	}
	else
		return nullptr; // Не удалось создать объект на карте - учетку игрока тоже не будем создавать
}
