#pragma once

#include <cstdint>

union player_points_info_u
{
	uint8_t data[5];
	struct
	{
		uint8_t player_index;
		int32_t player_points;
	} value;
};
