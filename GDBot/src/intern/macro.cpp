#include <macro.h>
#include <iostream>
#include <Windows.h>

#define IS_PRESSING MButton.mi.dwFlags == MOUSEEVENTF_LEFTDOWN

extern HWND hwnd;
extern HANDLE handle;
extern DWORD xAdress;

INPUT MButton;

Macro::Macro() {
	MButton.type			= INPUT_MOUSE;
	MButton.mi.dwFlags		= MOUSEEVENTF_LEFTUP;
	MButton.mi.dx			= 0;
	MButton.mi.time			= 0;
	MButton.mi.dy			= 0;
	MButton.mi.mouseData	= 0;
	MButton.mi.dwExtraInfo	= NULL;
}
Macro::Macro(std::vector<float> data) {
	MButton.type			= INPUT_MOUSE;
	MButton.mi.dwFlags		= MOUSEEVENTF_LEFTUP;
	MButton.mi.dx			= 0;
	MButton.mi.time			= 0;
	MButton.mi.dy			= 0;
	MButton.mi.mouseData	= 0;
	MButton.mi.dwExtraInfo	= NULL;
	this->data = data;
}
Macro::~Macro() {}

void Macro::load(std::ifstream &file) {
	float tmp;
	while (!file.eof()) {
		file >> tmp;
		data.push_back(tmp);
	}
}
void Macro::save(std::ofstream &file) {
	for (int i = 0; i < data.size(); i++) {
		file << data[i];
		file << ((i % 2) ? '\n' : ' '); // write alternating SPACE or NEW_LINE
	}
}

void Macro::run() {
	float pos = 0;
	float next;
	for (unsigned i = 0; i < data.size(); i++) {
		next = data[i];
		do {
			ReadProcessMemory(handle, (LPCVOID)xAdress, &pos, sizeof(pos), NULL);
		} while (pos < next);
		std::cout << next << std::endl;

		MButton.mi.dwFlags = IS_PRESSING ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &MButton, sizeof(MButton));
	}
	std::cout << "\nEND OF MACRO... [ENTER]" << std::endl;
	std::cin.get();
}
void Macro::record() {
	unsigned old = data.size();
	float pos = 0;
	bool isPressing = false;

	while (!(GetKeyState(VK_F10) & 0x8000)) {
		if (GetKeyState(VK_F5) & 0x8000) data.erase(data.begin() + old, data.end());
		if (GetForegroundWindow() == hwnd) {
			if ((bool)(GetKeyState(VK_LBUTTON) & 0x8000) != isPressing) {
				ReadProcessMemory(handle, (LPCVOID)xAdress, &pos, sizeof(pos), NULL);
				data.push_back(pos);
				std::cout << pos << std::endl;

				isPressing = !isPressing;
			}
		}
	}
}

bool Macro::isEmpty() {
	return data.empty();
}