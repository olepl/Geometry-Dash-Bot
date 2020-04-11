#include <macro.h>
#include <utils.h>
#include <iostream>
#include <iomanip>
#include <Windows.h>

#define IS_PRESSING MButton.mi.dwFlags == MOUSEEVENTF_LEFTDOWN

extern HANDLE	hConsole;
extern HWND		hwnd;
extern HANDLE	handle;
extern DWORD	pos_address;
extern DWORD	att_address;
extern DWORD	freeze_address;
extern INPUT	MButton;
extern float	pos_buffer;
extern int		att_buffer;
extern int		freeze_buffer;

Macro::Macro() {}
Macro::Macro(std::vector<float> data) : data(data) {}
Macro::~Macro() {}

int Macro::load()
{
	std::ifstream file(name);
	if (!file.is_open()) return 1;

	float tmp;
	while (!file.eof())
	{
		file >> tmp;
		data.push_back(tmp);
	}
	return 0;
}
int Macro::save()
{
	std::ofstream file(name);
	if (!file.is_open()) return 1;

	for (int i = 0; i < data.size(); i++)
	{
		file << data[i];
		file << ((i % 2) ? '\n' : ' '); // write alternating SPACE or NEW_LINE
	}
	return 0;
}

void Macro::run()
{
	float nextEntry;
	for (unsigned index = 0; index < data.size(); index++)
	{
		nextEntry = data[index];
		do {
			if (KEYPRESS_SAVE) return;
			if (KEYPRESS_RESTART) index = 0;
			ReadProcessMemory(handle, (LPCVOID)pos_address, &pos_buffer, sizeof(pos_buffer), NULL);
		} while (pos_buffer < nextEntry);

		if (index % 2 == 0) std::cout << "(" << index/2 + 1 << ") " << nextEntry;
		else std::cout << " - " << nextEntry << std::endl;

		MButton.mi.dwFlags = IS_PRESSING ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &MButton, sizeof(MButton));
	}
	std::cout << "\nEND OF MACRO... [ENTER]" << std::endl;
	std::cin.get();
}
void Macro::record()
{
	unsigned old = data.size();
	bool isPressing = false;
	int index = 0;

	while (!KEYPRESS_SAVE)
	{
		if (KEYPRESS_RESTART) 
		{
			data.erase(data.begin() + old, data.end());
			index = 0;
		}
		if (GetForegroundWindow() == hwnd)
		{
			if ((bool)KEYPRESS_CLICK != isPressing)
			{
				ReadProcessMemory(handle, (LPCVOID)pos_address, &pos_buffer, sizeof(pos_buffer), NULL);
				data.push_back(pos_buffer);
				
				isPressing = !isPressing;

				if (isPressing) std::cout << "(" << ++index << ") " << pos_buffer;
				else std::cout << " - " << pos_buffer << std::endl;
			}
		}
	}
}

bool Macro::isEmpty()
{
	return data.empty();
}

void Macro::setName(std::string name)
{
	this->name = name;
}
std::string Macro::getName()
{
	return name;
}

void inspect()
{
	float prevPos	= 0;
	int index		= 0;
	int tmp = 999999999;

	while (!KEYPRESS_SAVE) {
		if (KEYPRESS_PAUSE)
		{
			ReadProcessMemory(handle, (LPCVOID)freeze_address, &freeze_buffer, sizeof(freeze_buffer), NULL);
			WriteProcessMemory(handle, (LPVOID)freeze_address, &tmp, sizeof(tmp), NULL);

			while (KEYPRESS_PAUSE);

			WriteProcessMemory(handle, (LPVOID)freeze_address, &freeze_buffer, sizeof(freeze_buffer), NULL);
		}
		ReadProcessMemory(handle, (LPCVOID)pos_address, &pos_buffer, sizeof(pos_buffer), NULL);
		if (pos_buffer != prevPos) 
		{
			if (pos_buffer < prevPos)
			{
				ReadProcessMemory(handle, (LPCVOID)att_address, &att_buffer, sizeof(att_buffer), NULL);
				std::cout << "#### ATTEMPT " << std::setw(3) << att_buffer << " ####" << std::endl;
				index = 0;
			}

			std::cout << "(" << std::setw(4) << std::left << ++index << ") ";
			std::cout << "[ " << std::setw(8) << std::left << pos_buffer << " ] ";

			if (KEYPRESS_CLICK)
			{
				SetConsoleTextAttribute(hConsole, 16*14 + 13);
				std::cout << "[#]";
			}
			else
			{
				SetConsoleTextAttribute(hConsole, 13);
				std::cout << "[ ]";
			}
			SetConsoleTextAttribute(hConsole, 7);

			std::cout << " => " << std::showpos << pos_buffer - prevPos << std::noshowpos << ", " << freeze_buffer << std::endl;

			prevPos	= pos_buffer;
		}
	}
}