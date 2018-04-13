#include <string>

#include "map_t.h"
#include "player_t.h"

player_t::player_t(const char * _login, const char * _password)
{
	strncpy_s(login, _login, 20);
	strncpy_s(password, _password, 20);
}

player_t::player_t(const char *_login, const char *_password, object_s *_object) :
	player_t(_login, _password)
{
	points = 0;
	object = _object;
}

player_t::player_t(const char * _login, const char * _password, int32_t _points) :
	player_t(_login, _password)
{
	points = _points;
	object = nullptr;
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
