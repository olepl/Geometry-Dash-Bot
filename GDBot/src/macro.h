#pragma once

#include <vector>
#include <fstream>

class Macro
{
public:
	Macro();
	Macro(std::vector<float> data);
	~Macro();

	int load();
	int save();

	void run();
	void record();

	bool isEmpty();

	void setName(std::string name);
	std::string getName();

private:
	std::vector<float>	data;
	std::string			name;
};

void inspect();