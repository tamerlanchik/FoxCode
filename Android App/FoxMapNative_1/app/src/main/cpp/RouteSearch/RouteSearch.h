#pragma once
#include "DijkstrasAlgorithm.h"
#include "GraphConstructor.h"

template<class T>
class RouteSearch{
public:
	RouteSearch(const std::vector<Hall> &Halls, const std::vector<Room> &Rooms);
	int RefreshData(const std::vector<Hall> &Halls, const std::vector<Room> &Rooms);
	int BuildRoute(int StartID, int EndID);
	const std::vector<int> &GetRoute();
	~RouteSearch();
private:
	bool ConvertRouteData();
	DijkstrasAlgorithm<T> SearchMethod;
	GraphConstructor<T> Graph;
	std::vector<int> Route;
};

