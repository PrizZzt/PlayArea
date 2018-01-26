#pragma once

#include <boost/asio.hpp>

class client_t
{
    boost::asio::io_context io_context;

    boost::asio::ip::tcp::socket socket;
    boost::asio::ip::tcp::resolver resolver;

    std::thread recv_thread;
    bool is_recv_thread_running;
    void recv_func();

    uint8_t size_x, size_y;
    uint8_t *map_string;
    uint32_t map_string_length = 0;

public:
    client_t();
    ~client_t();

    void connect();

    uint8_t  *get_map_string() { return map_string; }
    uint32_t  get_map_string_length() { return map_string_length; }

    void up();
    void down();
    void right();
    void left();
    void act();
};
