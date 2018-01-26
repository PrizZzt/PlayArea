#include "../PlayArea/contstants.h"
#include "client_t.h"

void client_t::recv_func()
{
    uint32_t new_length;

    while (is_recv_thread_running)
    {
        if (boost::asio::read(socket, boost::asio::buffer(&size_x, 1)) == 0)continue;
        if (boost::asio::read(socket, boost::asio::buffer(&size_y, 1)) == 0)continue;

        new_length = size_x * size_y;
        if (new_length != map_string_length)
        {
            if (map_string_length > 0)
                delete[] map_string;

            map_string_length = size_x * size_y;
            map_string = new uint8_t[map_string_length];
        }

        size_t reply_length = boost::asio::read(socket, boost::asio::buffer(map_string, map_string_length));
    }
}

client_t::client_t() :
    socket(io_context),
    resolver(io_context)
{
}

client_t::~client_t()
{
    is_recv_thread_running = false;
    recv_thread.join();

    if (socket.is_open())
        socket.close();

    delete[] map_string;
}

void client_t::connect()
{
    boost::asio::connect(socket, resolver.resolve("127.0.0.1", "1234"));

    is_recv_thread_running = true;
    recv_thread = std::thread(&client_t::recv_func, this);

    char request[MAX_LENGTH];
    char login[6] = "Login";
    char password[9] = "Password";
    request[0] = 6;
    for (int i = 0; i < 6; i++)request[i + 1] = login[i];
    for (int i = 0; i < 9; i++)request[i + 21] = password[i];
    boost::asio::write(socket, boost::asio::buffer(request, MAX_LENGTH));
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
