#include <string.h>
#include <iostream>

#include "client_t.h"
#include "console_helper_t.h"

int main()
{
    client_t client;
	console_helper_t console_helper;
	client.connect("Login", "Password");

	bool run = true;
    while (run)
    {
		console_helper.show(client);
		console_helper.process_events([&](WORD _event_type, bool _key_down, WORD _key_code, CHAR _ascii_code)
		{
			if (_event_type == KEY_EVENT)
			{
				if (_key_down)
				{
					switch (_key_code)
					{
					case VK_UP:client.up();break;
					case VK_DOWN:client.down();break;
					case VK_RIGHT:client.right();break;
					case VK_LEFT:client.left();break;
					case VK_SPACE:client.act(); break;
					case VK_ESCAPE:run = false; break;
					}
				}
			}
		});
    }

    return 0;
}
