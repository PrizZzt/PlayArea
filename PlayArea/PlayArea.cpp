#include <iostream>
#include <boost/asio.hpp>

#include "server_t.h"

int main()
{
#ifndef _DEBUG
    try
    {
#endif
        server_t s(1234);
#ifndef _DEBUG
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
#endif

    return 0;
}
