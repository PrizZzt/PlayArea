#include <iostream>
#include <boost/asio.hpp>

#include "bomberman_logic_t.h"
#include "server_t.h"

int main()
{
#ifndef _DEBUG
    try
    {
#endif
        bomberman_logic_t *logic = new bomberman_logic_t();
        server_t s(logic, 1234);
#ifndef _DEBUG
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
#endif

    return 0;
}
