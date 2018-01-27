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

		// TO DELETE
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

		for (uint32_t i = 2; i < map_string_length; i++)
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
			if ((i - 1) % map.get_size_x() == 0)std::cout << std::endl;
		}
    }
}

void server_t::do_accept()
{
    acceptor.async_accept([this](boost::system::error_code _ec, boost::asio::ip::tcp::socket _socket)
    {
        if (!_ec)
        {
            std::make_shared<session_t>(std::move(_socket), this)->start();
        }

        do_accept();
    });
}

server_t::server_t(short _port) :
    acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _port)),
    map(20, 20)
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
    for (uint16_t i = 0; i < players_count; i++)
    {
        if (players[i]->check_credentials(_login, _password))
        {
            return players[i];
        }
    }

    object_s *new_object = map.add_new_player_object();
    if (new_object)
    {
        players[players_count] = new player_t(_login, _password, new_object);
        players_count++;

		return players[players_count - 1];
    }
    else
        return nullptr; // �� ������� ������� ������ �� ����� - ������ ������ ���� �� ����� ���������
}
