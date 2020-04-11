#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>
#include <macro.h>

class Header {
public:
	Header(Macro* macro, DWORD* address);
	~Header();

	void update();

	std::string getStatus();
	void setStatus(std::string status);
	void setHotkeys(byte hotkeys);

private:
	Macro*	macro;
	DWORD*	address;
	std::string	status;
	byte	hotkeys; // [][][SAVE][][INSPECT][RECORD][RUN][RESTART]
};

void cls();

// config
#define KEYPRESS_RESTART	(GetKeyState(VK_F5) & 0x8000)
#define KEYPRESS_RUN		(GetKeyState(VK_F6) & 0x8000)
#define KEYPRESS_RECORD		(GetKeyState(VK_F7) & 0x8000)
#define KEYPRESS_INSPECT	(GetKeyState(VK_F8) & 0x8000)
#define KEYPRESS_PAUSE		(GetKeyState(VK_F9) & 0x8000)
#define KEYPRESS_SAVE		(GetKeyState(VK_F10) & 0x8000)
#define KEYPRESS_CLICK		(GetKeyState(VK_LBUTTON) & 0x8000)

#define HOTKEYLIST_SLOT_RESTART	0x01
#define HOTKEYLIST_SLOT_RUN		0x02
#define HOTKEYLIST_SLOT_RECORD	0x04
#define HOTKEYLIST_SLOT_INSPECT 0x08
#define HOTKEYLIST_SLOT_SAVE	0x20
#define HOTKEYLIST_SLOT_ALL		0xFF