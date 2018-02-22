#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <functional>

#include "console_helper_t.h"
#include "client_t.h"
#include "bomberman_objects_e.h"

console_helper_t::console_helper_t()
{
	setlocale(LC_ALL, "Russian");
	std::cout.setf(std::ios::left);
	input_handle = GetStdHandle(STD_INPUT_HANDLE);
	output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	/*
	SMALL_RECT sr;
	sr.Top = 0;
	sr.Left = 0;
	sr.Bottom = 49;
	sr.Right = 79;
	if (SetConsoleWindowInfo(output_handle, true, &sr) == 0)throw new std::exception();

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);

	cfi.dwFontSize.X = 8;
	cfi.dwFontSize.Y = 9;
	cfi.nFont = 0;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"");

	SetCurrentConsoleFontEx(output_handle, false, &cfi);
	*/
}

console_helper_t::~console_helper_t()
{
}

void console_helper_t::gotoxy(short x, short y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(output_handle, c);
}

void console_helper_t::clrscr()
{
	COORD coord = { 0 };
	CONSOLE_SCREEN_BUFFER_INFO cBufferInfo;
	DWORD dwI;
	DWORD dwSize;

	if (output_handle == INVALID_HANDLE_VALUE) return;
	if (GetConsoleScreenBufferInfo(output_handle, &cBufferInfo))
	{
		dwSize = cBufferInfo.dwSize.X * cBufferInfo.dwSize.Y;
		FillConsoleOutputCharacter(output_handle, TEXT(' '), dwSize, coord, &dwI);
		SetConsoleCursorPosition(output_handle, coord);
	}
}

void console_helper_t::show(client_t &_client)
{
	gotoxy(0, 0);

	std::unique_lock<std::mutex> lock(_client.get_map_string_mutex());

	for (uint32_t i = 0; i < _client.get_map_string_length(); i += 2)
	{
		switch ((bomberman_objects_e)_client.get_map_string()[i])
		{
		case bomberman_objects_e::NONE: std::cout << ' '; break;
		case bomberman_objects_e::UNDESTRUCTIBLE_WALL:
		case bomberman_objects_e::DESTRUCTIBLE_WALL: std::cout << '#'; break;
		case bomberman_objects_e::DESTROYED_WALL: std::cout << '%'; break;
		case bomberman_objects_e::PLAYER: std::cout << 'P'; break;
		case bomberman_objects_e::PLAYER_WITH_BOMB_1: std::cout << '1'; break;
		case bomberman_objects_e::PLAYER_WITH_BOMB_2: std::cout << '2'; break;
		case bomberman_objects_e::PLAYER_WITH_BOMB_3: std::cout << '3'; break;
		case bomberman_objects_e::PLAYER_WITH_BOMB_4: std::cout << '4'; break;
		case bomberman_objects_e::PLAYER_WITH_BOMB_5: std::cout << '5'; break;
		case bomberman_objects_e::DEAD_PLAYER: std::cout << 'X'; break;

		case bomberman_objects_e::MEAT_CHOPPER: std::cout << 'M'; break;
		case bomberman_objects_e::DEAD_MEAT_CHOPPER: std::cout << 'W'; break;

		case bomberman_objects_e::BOOM: std::cout << '*'; break;
		case bomberman_objects_e::BOMB_1: std::cout << '1'; break;
		case bomberman_objects_e::BOMB_2: std::cout << '2'; break;
		case bomberman_objects_e::BOMB_3: std::cout << '3'; break;
		case bomberman_objects_e::BOMB_4: std::cout << '4'; break;
		case bomberman_objects_e::BOMB_5: std::cout << '5'; break;

		default:
			std::cout << (char)_client.get_map_string()[i];
			break;
		}
		if ((i / 2 + 1) % _client.get_size_x() == 0)std::cout << std::endl;
	}
	std::cout << std::endl;
}

void console_helper_t::process_events(std::function<void(WORD event_type, bool _key_down, WORD key_code, CHAR ascii_code)> _handler)
{
	GetNumberOfConsoleInputEvents(input_handle, &events_count);
	if (events_count > 0)
	{
		ReadConsoleInput(input_handle, event_buffer, events_count, &events_count_readed);
		for (DWORD i = 0; i < events_count_readed; ++i)
		{
			_handler(
				event_buffer[i].EventType,
				event_buffer[i].Event.KeyEvent.bKeyDown,
				event_buffer[i].Event.KeyEvent.wVirtualKeyCode,
				event_buffer[i].Event.KeyEvent.uChar.AsciiChar
			);
		}
	}
}
