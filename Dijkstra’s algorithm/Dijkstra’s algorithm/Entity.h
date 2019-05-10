#pragma once
#include <iostream>
#include <string>
#include <vector>

struct Coordinate {
	int x;
	int y;
	int z;
};

struct Hall {
	int ID;
	Coordinate LeftTop;
	Coordinate RightBottom;
	bool Status; //Работает или нет
};

struct Room :Hall {
	std::vector<Coordinate> Input;
	std::vector<int> Wight; //Под вопросом, мб не будет и везде будет стандарная ширина дверного проёма
	std::vector<int> HallID;
	std::string Type; //Аудитория, лифт, лестница, столовая и т. д.
};
