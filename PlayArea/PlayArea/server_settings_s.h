#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

struct server_settings_s
{
	short port = 1234;
	uint32_t save_players_period = 60;

	struct map_settings_s
	{
		uint8_t size_x = 20;
		uint8_t size_y = 20;
	} map_settings;

	void load(const std::string &_filename)
	{
		if (boost::filesystem::exists(_filename))
		{
			boost::property_tree::ptree tree;
			boost::property_tree::read_json(_filename, tree);

			port = tree.get<short>("server.port", port);
			save_players_period = tree.get<uint32_t>("server.save_players_period", save_players_period);
			map_settings.size_x = tree.get<uint8_t>("server.map.size_x", map_settings.size_x);
			map_settings.size_y = tree.get<uint8_t>("server.map.size_y", map_settings.size_y);
		}
	}

	void save(const std::string &_filename)
	{
		boost::property_tree::ptree tree;

		tree.put("server.port", port);
		tree.put("server.save_players_period", save_players_period);
		tree.put("server.map.size_x", map_settings.size_x);
		tree.put("server.map.size_y", map_settings.size_y);

		boost::property_tree::write_json(_filename, tree);
	}
};
