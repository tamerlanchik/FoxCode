#pragma once
#include "Entity.h"

template<class T>
class GraphConstructor{
public:
	GraphConstructor(const std::vector<Hall> &halls,const std::vector<Room> &rooms);
	~GraphConstructor();
	std::vector<std::vector<T>> GetMatrix();
	int CreateGraphMatrix();
private:
	const std::vector<Hall> Halls;
	const std::vector<Room> Rooms;
	std::vector<std::vector<T>> Matrix;
	int MatrixSize;
};

