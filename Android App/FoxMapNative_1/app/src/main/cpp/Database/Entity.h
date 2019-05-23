#pragma once
#include <iostream>
#include <string>
#include <vector>

//Структура с данными о приложении (и о БД)
struct Info {
	std::string Version;
	std::string CreationDate;
};

struct Coordinate {
	int x;
	int y;
	int z;
};

struct Hall {
	std::string ID;
	Coordinate LeftTop;
	Coordinate RightBottom;	
	std::vector<std::string> HallID;
	bool Status; //Работает или нет
};

struct Room :Hall {
	std::vector<Coordinate> Input;
	std::vector<int> Wight; //Под вопросом, мб не будет и везде будет стандарная ширина дверного проёма
	std::string Type; //Аудитория, лифт, лестница, столовая и т. д.
};
