#pragma once

#include <memory>
#include <boost/asio.hpp>

#include "contstants.h"
#include "player_t.h"

class server_t;

class session_t : public std::enable_shared_from_this<session_t>
{
    boost::asio::ip::tcp::socket socket;
    uint8_t data[MAX_LENGTH];
    uint8_t result[MAX_LENGTH_SERVER];
    server_t *server;
    player_t *player;

    void do_read();
		void do_write(uint32_t _length);

public:
    session_t(boost::asio::ip::tcp::socket _socket, server_t *_server);
    ~session_t();

    void start();
    void send_map(uint8_t *_map, uint32_t _map_lenght);
};
