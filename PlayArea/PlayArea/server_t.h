#pragma once

#include <boost/asio.hpp>

#include "session_t.h"
#include "map_t.h"
#include "player_t.h"
#include "game_logic_t.h"
#include "server_settings_s.h"

class server_t
{
    boost::asio::io_context        io_context;
    boost::asio::ip::tcp::acceptor acceptor;

    map_t     map;
    uint8_t  *map_string;
    uint32_t  map_string_length;

		player_t *players[MAX_PLAYERS_COUNT];
    uint8_t  players_count;
    std::list<std::shared_ptr<session_t>> sessions;
		uint32_t ticks_to_save_players;// Тиков до сохранения игроков в файл осталось
		uint32_t save_players_period;// Тиков до сохранения игроков в файл

    std::thread update_thread;
    bool is_update_thread_running;
    void update_func();

    void do_accept();

public:
    server_t(game_logic_t *_game_logic, server_settings_s &_settings, player_list_s &_player_list);
    ~server_t();

    uint8_t  *get_map_string() { return map_string; }
    uint32_t  get_map_string_length() { return map_string_length; }
		uint8_t   get_players_count() { return players_count; }
		char     *get_player_name(uint8_t _player_index) { if (_player_index < players_count)return players[_player_index]->get_player_name(); else return nullptr; }
		int32_t   get_player_points(uint8_t _player_index) { if (_player_index < players_count)return players[_player_index]->get_points(); else return 0; }

    player_t *get_player(char *_login, char *_password);

    void detach_session(std::shared_ptr<session_t> _session)
    {
        sessions.remove(_session);
    }
};
