#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

struct server_settings_s
{
	short port;

	struct map_settings_s
	{
		uint8_t size_x;
		uint8_t size_y;
	} map_settings;

	void load(const std::string &_filename)
	{
		boost::property_tree::ptree tree;
		boost::property_tree::read_json(_filename, tree);

		port = tree.get<short>("server.port", 1234);
		map_settings.size_x = tree.get<uint8_t>("server.map.size_x", 20);
		map_settings.size_y = tree.get<uint8_t>("server.map.size_y", 20);
	}

	void save(const std::string &_filename)
	{
		boost::property_tree::ptree tree;

		tree.put("server.port", port);
		tree.put("server.map.size_x", map_settings.size_x);
		tree.put("server.map.size_y", map_settings.size_y);

		boost::property_tree::write_json(_filename, tree);
	}
};
