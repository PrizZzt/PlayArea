#include <iostream>
#include <boost/asio.hpp>
#include <boost/dll/import.hpp>

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
		boost::dll::shared_library lib(settings.game_logic_name, boost::dll::load_mode::append_decorations);
		if (lib.has("logic") == false)
			throw std::exception("No game logic was loaded");
		boost::shared_ptr<game_logic_t> logic = boost::dll::import<game_logic_t>(lib, "logic");

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
