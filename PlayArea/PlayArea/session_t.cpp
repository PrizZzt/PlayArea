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

        switch ((client_action_e)data[0])
        {
        case client_action_e::MOVE_UP:
        case client_action_e::MOVE_DOWN:
        case client_action_e::MOVE_RIGHT:
        case client_action_e::MOVE_LEFT:
        case client_action_e::ACT:
            if (player)player->set_object_action((client_action_e)data[0]);
            break;

        case client_action_e::CREDENTIALS:
            player = server->get_player((char*)data + 1, (char*)data + 21);
            break;
        }

        do_write();
    });
}

void session_t::do_write()
{
    result[1] = 0;
    auto self(shared_from_this());
    boost::asio::async_write(socket, boost::asio::buffer(result, 2), [this, self](boost::system::error_code _ec, std::size_t /*length*/)
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
    result[0] = (uint8_t)server_action_e::MESSAGE;
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
