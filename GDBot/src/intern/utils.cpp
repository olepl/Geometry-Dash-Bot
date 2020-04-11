#include <utils.h>

extern HANDLE hConsole;

Header::Header(Macro* macro, DWORD* address) : macro(macro), address(address) {
	status = "-";
}
Header::~Header() {}

void Header::update() {
	CONSOLE_SCREEN_BUFFER_INFO buffer;
	GetConsoleScreenBufferInfo(hConsole, &buffer);
	COORD origin = { 0, 0 };

	// clearing the first two lines
	DWORD written;
	DWORD cells = buffer.dwSize.X * (DWORD)2;
	FillConsoleOutputCharacter(hConsole, ' ', cells, origin, &written);
	FillConsoleOutputAttribute(hConsole, buffer.wAttributes, cells, origin, &written);

	// writing new stuff
	SetConsoleCursorPosition(hConsole, origin);

	std::cout << "(Address: ";
	std::cout << std::hex << *address << std::dec;
	std::cout << " ) [" << status << "] => " << macro->getName() << ";" << std::endl;

	if (hotkeys & 1 << 0) std::cout << "RESTART F5; ";
	if (hotkeys & 1 << 1) std::cout << "RUN F6; ";
	if (hotkeys & 1 << 2) std::cout << "RECORD F7; ";
	if (hotkeys & 1 << 3) std::cout << "INSPECT F8; ";
	if (hotkeys & 1 << 5) std::cout << "SAVE F10; ";

	SetConsoleCursorPosition(hConsole, buffer.dwCursorPosition);
}

std::string Header::getStatus() {
	return status;
}
void Header::setStatus(std::string status)
{
	this->status = status;
	update();
}

void Header::setHotkeys(byte hotkeys) {
	this->hotkeys = hotkeys;
}

void cls()
{
	COORD origin = { 0, 0 };
	DWORD written;

	CONSOLE_SCREEN_BUFFER_INFO	buffer;
	GetConsoleScreenBufferInfo(hConsole, &buffer);

	DWORD cells = buffer.dwSize.X * buffer.dwSize.Y;

	FillConsoleOutputCharacter(hConsole, ' ', cells, origin, &written);
	FillConsoleOutputAttribute(hConsole, buffer.wAttributes, cells, origin, &written);
	SetConsoleCursorPosition(hConsole, origin);
	std::cout << "\n\n";
}