#pragma once
#include <iostream>
#include <string>

struct Coordinate {
	int x;
	int y;
};

struct Hall {
	int ID;
	Coordinate TopLeft;
	Coordinate RightBottom;
	int Floor;
	bool Status;
};

struct Room :Hall {
	Coordinate* Input;
	int* Wight; //��� ��������, �� �� ����� � ����� ����� ���������� ������ �������� �����
	int InputNumber;
	int* CorridorID;
	int CorridorsNumber;
	std::string Type;
};

struct MoveObject :Room {
	int MinFloor;
	int MaxFloor;
};