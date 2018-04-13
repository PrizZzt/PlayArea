#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <list>

struct player_list_s
{
	struct player_info_s
	{
		std::string login;
		std::string password;
		int32_t points;
	};

	std::list<player_info_s> player_infos;

	void load(const std::string &_filename)
	{
		if (boost::filesystem::exists(_filename))
		{
			boost::property_tree::ptree tree;
			boost::property_tree::read_json(_filename, tree);

			for (auto &player_tree : tree.get_child("players"))
			{
				player_info_s player_info;
				player_info.login = player_tree.second.get<std::string>("login");
				player_info.password = player_tree.second.get<std::string>("password");
				player_info.points = player_tree.second.get<int32_t>("points");

				player_infos.push_back(player_info);
			}
		}
	}

	void save(const std::string &_filename)
	{
		boost::property_tree::ptree tree;

		for (auto &player_info : player_infos)
		{
			boost::property_tree::ptree player_tree;
			player_tree.put("login", player_info.login);
			player_tree.put("password", player_info.password);
			player_tree.put("points", player_info.points);

			tree.add_child("players.player", player_tree);
		}

		boost::property_tree::write_json(_filename, tree);
	}
};