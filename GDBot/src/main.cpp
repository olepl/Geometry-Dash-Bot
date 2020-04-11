#include <iostream>
#include <string>
#include <fstream>
#include <memory.h>
#include <utils.h>
#include <macro.h>

#define UPDATE_ADRESSES	base_address	= getBaseAddress(_T("GeometryDash.exe")); \
						pos_address		= getAddress({ 0x3222D0, 0x164, 0x124, 0xEC, 0x108, 0x67C	}); \
						att_address		= getAddress({ 0x3222D0, 0x164, 0x20C, 0xB4, 0xB4, 0x4A8	}); \
						freeze_address	= getAddress({ 0x3222D0, 0x164, 0x484, 0xB4, 0x224, 0x660	});

HANDLE	hConsole;
INPUT	MButton;
std::string	input = "";

HWND	hwnd;
DWORD	procID;
HANDLE	handle;

DWORD	base_address;
DWORD	pos_address;
DWORD	att_address;
DWORD	freeze_address;
float	pos_buffer = 0;
int		att_buffer = 0;
int		freeze_buffer = 0;
Macro	macro;
Header	header(&macro, &att_address);

void Init()
{
	hConsole	= GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle(_T("Geometry Dash Bot by BooleanGD [v1.2.0]"));

	MButton.type			= INPUT_MOUSE;
	MButton.mi.dwFlags		= MOUSEEVENTF_LEFTUP;
	MButton.mi.dx			= 0;
	MButton.mi.time			= 0;
	MButton.mi.dy			= 0;
	MButton.mi.mouseData	= 0;
	MButton.mi.dwExtraInfo	= NULL;

	hwnd		= FindWindow(NULL, _T("Geometry Dash"));
	GetWindowThreadProcessId(hwnd, &procID);
	handle		= OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

	UPDATE_ADRESSES

	macro.setName("-");
}

void Menu()
{
	cls();

	header.setStatus("MENU");
	header.setHotkeys(HOTKEYLIST_SLOT_RUN | HOTKEYLIST_SLOT_RECORD | HOTKEYLIST_SLOT_INSPECT);

	header.update();

	while (true)
	{
		if (KEYPRESS_RUN)
		{
			while (macro.isEmpty())
			{
				std::cout << "Type in which macro to run:\n> ";

				getline(std::cin, input);
				macro.setName(input);

				if (macro.load())
				std::cout << "Sorry, this macro does not exist yet." << std::endl;
			}

			header.setStatus("RUNNING");
			header.setHotkeys(HOTKEYLIST_SLOT_RESTART | HOTKEYLIST_SLOT_SAVE);
			UPDATE_ADRESSES

			header.update();

			macro.run();

			return;
		}
		else if (KEYPRESS_RECORD)
		{
			header.setStatus("RECORDING");
			header.setHotkeys(HOTKEYLIST_SLOT_RESTART | HOTKEYLIST_SLOT_SAVE);
			UPDATE_ADRESSES

			header.update();

			macro.record();

			std::cout << "Type in the name to save the macro\n> ";

			getline(std::cin, input);
			macro.setName(input);
			macro.save();
			
			return;
		}
		else if (KEYPRESS_INSPECT)
		{ 
			header.setStatus("INSPECTING");
			header.setHotkeys(HOTKEYLIST_SLOT_SAVE);
			UPDATE_ADRESSES

			header.update();

			inspect();

			return;
		}
	}
}

int main(int argc, char* argv[])
{
	Init();

	while (1)
	{
		Menu();
	}
	

	return 0;
}