#include <string.h>
#include <iostream>

#include "client_t.h"

int main()
{
    //client_t client;
    //client.connect();

    int key;
    while (true)
    {
        key = std::cin.get();
        std::cout << std::to_string(key) << std::endl;
        

        /*for (uint32_t i = 0; i < reply_length; i++)
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
        std::cout << std::endl;*/
    }

    return 0;
}
