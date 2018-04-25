#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

struct bomberman_settings_s
{
	int32_t wall_score = 10;
	int32_t meatchopper_score = 100;
	int32_t player_score = 1000;
	int32_t player_dead_penalty = 1;
	int32_t player_score_min = -10;
	int32_t player_score_max = INT32_MAX - 1;

	void load(const std::string &_filename)
	{
		if (boost::filesystem::exists(_filename))
		{
			boost::property_tree::ptree tree;
			boost::property_tree::read_json(_filename, tree);

			wall_score = tree.get<int32_t>("bomberman.wall_score", wall_score);
			meatchopper_score = tree.get<int32_t>("bomberman.meatchopper_score", meatchopper_score);
			player_score = tree.get<int32_t>("bomberman.player_score", player_score);
			player_dead_penalty = tree.get<int32_t>("bomberman.player_dead_penalty", player_dead_penalty);
			player_score_min = tree.get<int32_t>("bomberman.player_score_min", player_score_min);
			player_score_max = tree.get<int32_t>("bomberman.player_score_max", player_score_max);
		}
	}

	void save(const std::string &_filename)
	{
		boost::property_tree::ptree tree;

		tree.put("bomberman.wall_score", wall_score);
		tree.put("bomberman.meatchopper_score", meatchopper_score);
		tree.put("bomberman.player_score", player_score);
		tree.put("bomberman.player_dead_penalty", player_dead_penalty);
		tree.put("bomberman.player_score_min", player_score_min);
		tree.put("bomberman.player_score_max", player_score_max);

		boost::property_tree::write_json(_filename, tree);
	}
};
