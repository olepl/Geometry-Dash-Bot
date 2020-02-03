#pragma once

#include <vector>
#include <fstream>

//requires HWND hwnd, HANDLE handle, DWORD xAdress
class Macro {
public:
	Macro();
	Macro(std::vector<float> data);
	~Macro();

	void load(std::ifstream &file);
	void save(std::ofstream &file);

	void run();
	void record();

	bool isEmpty();

private:
	std::vector<float> data;
};