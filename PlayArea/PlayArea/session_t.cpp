#include "server_t.h"
#include "server_action_e.h"
#include "session_t.h"

void session_t::do_read()
{
    auto self(shared_from_this());
    socket.async_read_some(boost::asio::buffer(data, MAX_LENGTH), [this, self](boost::system::error_code _ec, std::size_t _length)
    {
        if (_ec)
        {
            server->detach_session(self);
            return;
        }

				uint16_t position;
				switch ((client_action_e)data[0])
				{
				case client_action_e::MOVE_UP:
				case client_action_e::MOVE_DOWN:
				case client_action_e::MOVE_RIGHT:
				case client_action_e::MOVE_LEFT:
				case client_action_e::ACT:
					if (player)player->set_object_action((client_action_e)data[0]);
					result[0] = (uint8_t)server_action_e::MESSAGE_OK;
					result[1] = 0;
					do_write(2);
					break;

				case client_action_e::CREDENTIALS:
					player = server->get_player((char*)data + 1, (char*)data + 21);
					if (player && player->object)
					{
						result[0] = (uint8_t)server_action_e::MESSAGE_OK;
						result[1] = player->object->id;
					}
					else
					{
						result[0] = (uint8_t)server_action_e::MESSAGE_FAIL;
						result[1] = 0;
					}
					do_write(2);
					break;

				case client_action_e::GET_NAMES:
					result[0] = (uint8_t)server_action_e::NAMES_LIST;
					result[1] = server->get_players_count();
					position = 2;
					for (uint8_t i = 0; i < result[1]; i++)
					{
						char *login = server->get_player_name(i);
						result[position] = i;
						position++;
						result[position] = (uint8_t)strlen(login);
						position++;
						for (uint8_t j = 0; j < 20; j++)
						{
							if (login[j] == 0)break;
							result[position] = login[j];
							position++;
						}
					}
					do_write(position);
					break;

				case client_action_e::GET_POINTS:
					result[0] = (uint8_t)server_action_e::POINTS_LIST;
					result[1] = server->get_players_count();
					position = 2;
					for (uint8_t i = 0; i < result[1]; i++)
					{
						int32_t points = server->get_player_points(i);
						result[position] = i;
						position++;
						result[position] = (points >> 24) & 0xff;
						position++;
						result[position] = (points >> 16) & 0xff;
						position++;
						result[position] = (points >> 8) & 0xff;
						position++;
						result[position] = points & 0xff;
						position++;
					}
					do_write(position);
					break;
				}
    });
}

void session_t::do_write(uint32_t _length)
{
	auto self(shared_from_this());
	boost::asio::async_write(socket, boost::asio::buffer(result, _length), [this, self](boost::system::error_code _ec, std::size_t /*length*/)
	{
		if (_ec)
		{
			server->detach_session(self);
			return;
		}

		do_read();
	});
}

session_t::session_t(boost::asio::ip::tcp::socket _socket, server_t *_server) :
    socket(std::move(_socket)),
    server(_server),
    player(nullptr)
{
}

session_t::~session_t()
{
	if (socket.is_open())
		socket.close();

	if (player && player->object)
	{
		player->object->to_delete = true;
		player->object = nullptr;
	}
}

void session_t::start()
{
    do_read();
}

void session_t::send_map(uint8_t *_map, uint32_t _map_lenght)
{
    boost::asio::async_write(
        socket,
        boost::asio::buffer(_map, _map_lenght),
        [this](boost::system::error_code _ec, std::size_t /*length*/){}
    );
}
