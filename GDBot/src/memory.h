#pragma once

#include <tchar.h>
#include <Windows.h>
#include <vector>

//requires DWROD procID
DWORD getBaseAdress(const TCHAR* moduleName);
//requires DWORD baseAdress, HANDLE handle
DWORD getAdress(std::vector<DWORD> offsets);