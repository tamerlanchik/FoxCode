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
	std::string ID;
	Coordinate LeftTop;
	Coordinate RightBottom;	
	std::vector<std::string> HallID;
	bool Status; //�������� ��� ���
};

struct Room :Hall {
	std::vector<Coordinate> Input;
	std::vector<int> Wight; //��� ��������, �� �� ����� � ����� ����� ���������� ������ �������� �����
	std::string Type; //���������, ����, ��������, �������� � �. �.
};
