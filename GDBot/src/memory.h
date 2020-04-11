#pragma once

#include <tchar.h>
#include <Windows.h>
#include <vector>

DWORD getBaseAddress(const TCHAR* moduleName);
DWORD getAddress(std::vector<DWORD> offsets);