#include <string.h>
#include <iostream>
#include <random>

#include "game_client_t.h"

bool is_block(bomberman_objects_e block)
{
	return
		block != bomberman_objects_e::NONE &&
		block != bomberman_objects_e::DESTROYED_WALL &&
		block != bomberman_objects_e::DEAD_PLAYER &&
		block != bomberman_objects_e::DEAD_MEATCHOPPER &&
		block != bomberman_objects_e::BOOM;
}

int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 4);
	game_client_t client;
	client.on_turn = [&]()
	{
		switch (dist(gen))
		{
		case 0:
			if (client.get_player_y() > 0 && is_block(client.get_object(client.get_player_x(), client.get_player_y() - 1)) == false)
				client.up();
			break;

		case 1:
			if (client.get_player_x() + 1 < client.get_size_x() && is_block(client.get_object(client.get_player_x() + 1, client.get_player_y())) == false)
				client.right();
			break;

		case 2:
			if (client.get_player_y() + 1 < client.get_size_y() && is_block(client.get_object(client.get_player_x(), client.get_player_y() + 1)) == false)
				client.down();
			break;

		case 3:
			if (client.get_player_x() > 0 && is_block(client.get_object(client.get_player_x() - 1, client.get_player_y())) == false)
				client.left();
			break;

		case 4:
			client.act();
			break;
		}
	};
	client.run("127.0.0.1", 1234, "CppClient", "Password");

	return 0;
}
