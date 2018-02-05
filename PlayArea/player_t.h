#pragma once

#include <cstdint>

#include "object_s.h"

class map_t;

// Учетная запись игрока
class player_t
{
    char login[20];
    char password[20];

public:
    object_s * object; // Соответствующий игроку объект на карте, всегда есть

    player_t(char *_login, char *_password, object_s *_object);
    ~player_t();

    char *get_player_name() { return login; }
	void set_object_action(client_action_e _action) { if (object)object->next_action = _action; }

    bool check_credentials(char *_login, char *_password);
};
