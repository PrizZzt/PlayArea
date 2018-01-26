#include "server_t.h"
#include "session_t.h"

void session_t::do_read()
{
    auto self(shared_from_this());
    socket.async_read_some(boost::asio::buffer(data, MAX_LENGTH), [this, self](boost::system::error_code _ec, std::size_t _length)
    {
        if (_ec)return;

        switch ((action_e)data[0])
        {
        case action_e::MOVE_UP:
        case action_e::MOVE_DOWN:
        case action_e::MOVE_RIGHT:
        case action_e::MOVE_LEFT:
        case action_e::ACT:
            if (player)player->set_object_action((action_e)data[0]);
            break;

        case action_e::CREDENTIALS:
            player = server->get_player((char*)data + 1, (char*)data + 21);
            break;
        }

        do_write();
    });
}

void session_t::do_write()
{
    auto self(shared_from_this());
    boost::asio::async_write(socket, boost::asio::buffer(server->get_map_string(), server->get_map_string_length()), [this, self](boost::system::error_code _ec, std::size_t /*length*/)
    {
        if (!_ec)
        {
            do_read();
        }
    });
}

session_t::session_t(boost::asio::ip::tcp::socket _socket, server_t *_server) :
    socket(std::move(_socket)),
    server(_server),
    player(nullptr)
{
}

void session_t::start()
{
    do_read();
}
