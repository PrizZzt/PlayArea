#include "client_t.h"

#define MAX_LENGTH 41

void client_t::recv_func()
{
	uint32_t new_length;

	uint8_t server_action;
	uint8_t result_code;
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

		case 2://RESULT
			if (boost::asio::read(socket, boost::asio::buffer(&result_code, 1)) == 0)continue;
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
}

client_t::~client_t()
{
	if (socket.is_open())
		socket.close();

    is_recv_thread_running = false;
    recv_thread.join();

    delete[] map_string;
}

void client_t::connect(const char *_login, const char *_password)
{
	boost::asio::connect(socket, resolver.resolve("127.0.0.1", "1234"));

	is_recv_thread_running = true;
	recv_thread = std::thread(&client_t::recv_func, this);

	char request[MAX_LENGTH];
	request[0] = 6;
	for (int i = 0; i < strlen(_login) + 1; i++)request[i + 1] = _login[i];
	for (int i = 0; i < strlen(_password) + 1; i++)request[i + 21] = _password[i];
	boost::asio::write(socket, boost::asio::buffer(request, MAX_LENGTH));
}

void client_t::do_receive_size_x()
{
	socket.async_read_some(boost::asio::buffer(&size_x, MAX_LENGTH), [this](boost::system::error_code _ec, std::size_t _length)
	{
		if (_ec)return;

		do_receive_size_y();
	});
}

void client_t::do_receive_size_y()
{
	socket.async_read_some(boost::asio::buffer(&size_y, 1), [this](boost::system::error_code _ec, std::size_t _length)
	{
		if (_ec)return;

		uint32_t new_length;
		new_length = size_x * size_y;
		if (new_length != map_string_length)
		{
			if (map_string_length > 0)
				delete[] map_string;

			map_string_length = size_x * size_y;
			map_string = new uint8_t[map_string_length];
		}

		do_receive_map();
	});
}

void client_t::do_receive_map()
{
	socket.async_read_some(boost::asio::buffer(map_string, map_string_length), [this](boost::system::error_code _ec, std::size_t _length)
	{
		if (_ec)return;

		do_receive_size_x();
	});
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
