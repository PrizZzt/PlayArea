#pragma once

#include <cstdint>

#include "object_s.h"

// Учетная запись игрока
class player_t
{
    char login[20];
    char password[20];

    object_s *object; // Соответствующий игроку объект на карте, всегда есть

public:
    player_t(char *_login, char *_password, object_s *_object);
    ~player_t();

    char *get_player_name() { return login; }
    object_s *get_object() { return object; }
	void set_object_action(action_e _action) { if (object)object->next_action = _action; }

    bool check_credentials(char *_login, char *_password);
};
