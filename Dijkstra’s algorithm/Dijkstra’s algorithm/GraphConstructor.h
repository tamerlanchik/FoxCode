#pragma once
#include "Entity.h"

template<class T>
class GraphConstructor{
public:
	GraphConstructor(const struct Hall* halls, const int hallsNumber,
		const struct Room* rooms, const int roomsNumber,
		const struct MoveObject* moveObjects, const int moveObjectsNumber);
	~GraphConstructor();
	T** GetMatrix();
	int GetMatrixSize();
	bool CreateGraph();
private:
	const struct Hall* Halls;
	const int HallsNumber;
	const struct Room* Rooms;
	const int RoomsNumber;
	const struct MoveObject* MoveObjects;
	const int MoveObjectsNumber;
	T** Matrix;
	int MatrixSize;
};

