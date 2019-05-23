#pragma once
#include <vector>
#include <queue>
template <class T>
class DijkstrasAlgorithm
{
public:
	DijkstrasAlgorithm();
	DijkstrasAlgorithm(const int start, const int end, const T**const matrix, const int matrixSize);
	~DijkstrasAlgorithm();
	int* GetRoute();
	int GetRouteLen();
	bool InputStart(const int start);
	bool InputEnd(const int end);
	bool inputGraph(const T**const Matrix);
private:
	const T** const Matrix;
	const int MatrixSize;
	T* MinDistance;
	int MinDistanceLen;
	int RouteLen;
	int* Route;
	const int startNode;
	const int endNode;
	bool* VisitedNodes;
	bool CalculateMinDistance();
	bool FindRoute();
};

