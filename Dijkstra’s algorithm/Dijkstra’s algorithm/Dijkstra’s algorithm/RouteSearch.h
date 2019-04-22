#pragma once
#include "DijkstrasAlgorithm.h"
#include "GraphConstructor.h"

struct RouteCoordinate :Coordinate {
	int z;
};

template<class T>
class RouteSearch
{
public:
	RouteSearch(const struct Hall* halls, const int hallsNumber,
		const struct Room* rooms, const int roomsNumber,
		const struct MoveObject* moveObjects, const int moveObjectsNumber,
		const int start, const int end, const T**const Matrix);
	~RouteSearch();
	RouteCoordinate* GetRoute();
	int GetRouteLen();
private:
	bool ConvertRouteData();
	DijkstrasAlgorithm<T> SearchMethod;
	GraphConstructor<T> Graph;
	RouteCoordinate* Route;
	int RouteLen;
};

