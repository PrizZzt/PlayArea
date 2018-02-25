#pragma once

#include <boost/asio.hpp>

#include "session_t.h"
#include "map_t.h"
#include "player_t.h"
#include "game_logic_t.h"

class server_t
{
    boost::asio::io_context        io_context;
    boost::asio::ip::tcp::acceptor acceptor;

    map_t     map;
    uint8_t  *map_string;
    uint32_t  map_string_length;

		player_t *players[MAX_PLAYERS_COUNT];
    uint16_t  players_count;
    std::list<std::shared_ptr<session_t>> sessions;

    std::thread update_thread;
    bool is_update_thread_running;
    void update_func();

    void do_accept();

public:
    server_t(game_logic_t *_game_logic, short _port);
    ~server_t();

    uint8_t  *get_map_string() { return map_string; }
    uint32_t  get_map_string_length() { return map_string_length; }

    player_t *get_player(char *_login, char *_password);

    void detach_session(std::shared_ptr<session_t> _session)
    {
        sessions.remove(_session);
    }
};