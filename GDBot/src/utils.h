#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>

// requires HANDLE hConsole
void clearScreen();
void writeHeader(std::vector<std::string>	str, short n = 0, short off = 0);