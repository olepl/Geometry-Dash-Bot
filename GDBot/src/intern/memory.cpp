#include <memory.h>
#include <TlHelp32.h>

extern DWORD	procID;

DWORD getBaseAddress(const TCHAR* moduleName)
{
	DWORD   baseAdress		= 0;
	HANDLE  hSnapshot		= CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procID);
	MODULEENTRY32 moduleEntry32	= { 0 };
	moduleEntry32.dwSize	= sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &moduleEntry32))
	{
		do {
			if (_tcscmp(moduleEntry32.szModule, moduleName) == 0)
			{
				baseAdress = (DWORD)moduleEntry32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &moduleEntry32));
	}

	CloseHandle(hSnapshot);
	return baseAdress;
}

extern DWORD	base_address;
extern HANDLE	handle;

DWORD getAddress(std::vector<DWORD> offsets)
{
	DWORD address_buffer = base_address;
	for (unsigned i = 0; i < offsets.size() - 1; i++)
	{
		ReadProcessMemory(handle, (LPCVOID)((std::uint64_t)address_buffer + (std::uint64_t)offsets[i]), &address_buffer, sizeof(address_buffer), NULL);
	}
	return address_buffer + offsets.back();
}