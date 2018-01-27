#pragma once

class client_t;

class console_helper_t
{
	HANDLE input_handle = NULL;
	HANDLE output_handle = NULL;

	DWORD events_count;
	DWORD events_count_readed;
	INPUT_RECORD event_buffer[256];

public:
	console_helper_t();
	~console_helper_t();

	void gotoxy(short x, short y);
	void clrscr();
	void show(client_t &_client);

	void process_events(std::function<void(WORD event_type, bool _key_down, WORD key_code, CHAR ascii_code)> _handler);
};
