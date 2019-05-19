#pragma once
#include "DijkstrasAlgorithm.h"
#include "GraphConstructor.h"

template<class T>
class RouteSearch{
public:
	RouteSearch(const std::vector<Hall> &Halls, const std::vector<Room> &Rooms,
		const int StartId, const int EndId);
	~RouteSearch();
	const std::vector<int> &GetRoute();
	int GetRouteLen();
private:
	bool ConvertRouteData();
	DijkstrasAlgorithm<T> SearchMethod;
	GraphConstructor<T> Graph;
	std::vector<int> Route;
	int RouteLen;
};

