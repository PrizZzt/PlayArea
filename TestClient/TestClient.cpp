#include <string.h>
#include <iostream>
#include <boost/asio.hpp>

#include "../PlayArea/contstants.h"

void read_map(boost::asio::ip::tcp::socket *_socket)
{

}

int main()
{
    boost::asio::io_context io_context;

    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::connect(socket, resolver.resolve("127.0.0.1", "1234"));

    char request[MAX_LENGTH];
    char login[6] = "Login";
    char password[9] = "Password";
    request[0] = 6;
    for (int i = 0; i < 6; i++)request[i + 1] = login[i];
    for (int i = 0; i < 9; i++)request[i + 21] = password[i];
    boost::asio::write(socket, boost::asio::buffer(request, MAX_LENGTH));

    uint8_t size_x, size_y;
    uint8_t *map_string;
    uint32_t map_string_length = 0;
    uint32_t new_length;
    while (true)
    {
        std::cout << "Enter message: ";
        std::cin.get(request, MAX_LENGTH);
        size_t request_length = std::strlen(request);
        boost::asio::write(socket, boost::asio::buffer(request, request_length));

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
        if (reply_length != map_string_length)continue;
        std::cout << "Reply is(" << std::to_string(reply_length) << "): " << std::endl;

        for (uint32_t i = 0; i < reply_length; i++)
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
            if ((i + 1) % size_x == 0)std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
