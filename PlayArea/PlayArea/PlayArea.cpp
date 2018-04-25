#include <iostream>
#include <boost/asio.hpp>

#include "bomberman_logic_t.h"
#include "server_t.h"
#include "player_list_s.h"

int main()
{
	srand(time(0));

	server_settings_s settings;
	settings.load("settings.json");
	settings.save("settings.json");

	player_list_s player_list;
	player_list.load("player_list.json");

#ifndef _DEBUG
	try
	{
#endif
		bomberman_logic_t *logic = new bomberman_logic_t();
		server_t s(logic, settings, player_list);
#ifndef _DEBUG
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
#endif

	return 0;
}
