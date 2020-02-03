#include <memory.h>
#include <TlHelp32.h>

extern DWORD procID;

DWORD getBaseAdress(const TCHAR* moduleName) {
	DWORD baseAdress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procID);
	MODULEENTRY32 moduleEntry32 = { 0 };
	moduleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &moduleEntry32)) {
		do {
			if (_tcscmp(moduleEntry32.szModule, moduleName) == 0) {
				baseAdress = (DWORD)moduleEntry32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &moduleEntry32));
	}

	CloseHandle(hSnapshot);
	return baseAdress;
}

extern DWORD baseAdress;
extern HANDLE handle;

DWORD getAdress(std::vector<DWORD> offsets) {
	for (unsigned i = 0; i < offsets.size() - 1; i++) {
		ReadProcessMemory(handle, (LPCVOID)(baseAdress + offsets[i]), &baseAdress, sizeof(baseAdress), NULL);
	}
	return baseAdress + offsets.back();
}