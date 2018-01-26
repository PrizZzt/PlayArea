#include <string>

#include "player_t.h"

player_t::player_t(char *_login, char *_password, object_s *_object)
{
    strncpy_s(login, _login, 20);
    strncpy_s(password, _password, 20);

    object = _object;
}

player_t::~player_t()
{
    if (object)object->to_delete = true;
}

bool player_t::check_credentials(char *_login, char *_password)
{
    return
        std::strncmp(login, _login, 20) == 0 &&
        std::strncmp(password, _password, 20) == 0;
}
