#include <iostream>
#include <string>
#include <fstream>
#include <memory.h>
#include <utils.h>
#include <macro.h>

#define CLS clearScreen(); \
std::cout << "[X-Address: " << std::hex << std::uppercase << xAdress << std::nouppercase << std::dec << " ]" << "[Macro: " << macro_name << " ]" << std::endl; \
std::cout << "[F5] RESTART MACRO; [F6] RUN MACRO; [F7] RECORD MACRO; [F8] OPEN INSPECTOR; [F9] PLACE MARKER; [F10] SAVE/STOP MACRO;\n" << std::endl;

HANDLE hConsole;
std::string input = "";
std::string macro_name = "-";

HWND hwnd;
DWORD procID;
HANDLE handle;

DWORD baseAdress;
DWORD xAdress;

int main(int argc, char* argv[]) {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle(_T("Geometry Dash Bot by BooleanGD [v1.1.0]"));

	hwnd = FindWindow(NULL, _T("Geometry Dash"));
	GetWindowThreadProcessId(hwnd, &procID);
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

	baseAdress = getBaseAdress(_T("GeometryDash.exe"));
	xAdress = getAdress({ 0x3222D0, 0x164, 0x124, 0xEC, 0x108, 0x67C });
	Macro macro;

	main_menu:
	CLS;
	
	while (true) {
		
		if			(GetKeyState(VK_F6) & 0x8000) {
			while (macro.isEmpty()) {
				std::cout << "What macro do you want to run?" << std::endl;
				
				getline(std::cin, input);	

				std::ifstream file(input);
				if (file.is_open()) {
					macro.load(file);
					macro_name = input;

					file.close();
				}
				else {
					std::cout << "\n'" << input << "' does not exist";
					std::cin.get();
					CLS;
				}
			}
			std::cout << "[PLAYING]" << std::endl;
			macro.run();
			goto main_menu;

		} else if	(GetKeyState(VK_F7) & 0x8000) {
			std::cout << "[RECORDING]" << std::endl;
			macro.record();
			std::cout << "As what do you want to save this?" << std::endl;
			getline(std::cin, input);

			std::ofstream file(input);
			if (file.is_open()) {
				macro.save(file);
				macro_name = input;

				file.close();
			}
			goto main_menu;

		} else if	(GetKeyState(VK_F8) & 0x8000) {
			goto main_menu;

		}
	}
	return 0;
}