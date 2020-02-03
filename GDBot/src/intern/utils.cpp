#include <utils.h>

extern HANDLE hConsole;

void clearScreen() {
	COORD	origin = { 0, 0 };
	DWORD written;

	CONSOLE_SCREEN_BUFFER_INFO	buffer;
	GetConsoleScreenBufferInfo(hConsole, &buffer);

	DWORD cells = buffer.dwSize.X * buffer.dwSize.Y;

	FillConsoleOutputCharacter(hConsole, ' ', cells, origin, &written);
	FillConsoleOutputAttribute(hConsole, buffer.wAttributes, cells, origin, &written);
	SetConsoleCursorPosition(hConsole, origin);
}

void writeHeader(std::vector<std::string> str, short n, short off) {
	DWORD written_buffer;
	COORD origin = { 0, 0 };
	origin.Y += off;
	
	n += off;

	CONSOLE_SCREEN_BUFFER_INFO	buffer;
	GetConsoleScreenBufferInfo(hConsole, &buffer);

	if (str.size() != n) {
		//scroll
		SMALL_RECT scroll_rect;
		scroll_rect.Top = n;
		scroll_rect.Bottom = buffer.dwSize.Y - 1;
		scroll_rect.Left = 0;
		scroll_rect.Right = buffer.dwSize.X - 1;

		SMALL_RECT clip_rect = scroll_rect;
		if (str.size() < n) {
			clip_rect.Top = str.size();
		}

		COORD dest;
		dest.X = 0;
		dest.Y = str.size() + off;

		CHAR_INFO c;
		c.Char.UnicodeChar = (char)' ';
		c.Attributes = 0;

		ScrollConsoleScreenBuffer(hConsole, &scroll_rect, &clip_rect, dest, &c);
	}

	DWORD cells = buffer.dwSize.X * str.size();
	FillConsoleOutputCharacter(hConsole, ' ', cells, origin, &written_buffer);

	SetConsoleCursorPosition(hConsole, origin);
	for (int i = 0; i < str.size(); i++) {
		std::cout << str[i] << std::endl;
	}

	COORD cursor_pos = buffer.dwCursorPosition;
	cursor_pos.Y += str.size() - n + off;

	SetConsoleCursorPosition(hConsole, cursor_pos);
}