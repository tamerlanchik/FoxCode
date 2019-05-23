#pragma once
#include "IGraph.h"
#include "Database/Entity.h"

class CMatrixGraph :public IGraph {
public:
	CMatrixGraph(int count);
	CMatrixGraph(const IGraph& graph);
	CMatrixGraph(const std::vector<Hall> &halls, const std::vector<Room> &rooms);
	~CMatrixGraph();

	int RefreshData(const std::vector<Hall> &halls, const std::vector<Room> &rooms);

	virtual void AddEdge(int from, int to) override;
	virtual int VerticesCount() const override;

	virtual std::vector<int> GetNextVertices(int vertex) const override;
	virtual std::vector<int> GetPrevVertices(int vertex) const override;

	void ptintMatrix();
	int FindRoute(std::string StartID, std::string EndID);
	std::vector<std::string> GetLastRoute();

private:
	static int GetHallIndex(std::vector<Hall> Halls, std::string HallID);
	static int HallRoomDistance(Room room, Hall hall);
	static int HallHallDistance(Hall hall1, Hall hall2);
	static std::vector<int> Dijkstra(const std::vector<std::vector<int>> Matrix, int StartIndex);
	std::vector<std::string> LastRoute;
	std::string Error;
	std::vector<std::vector<int>> adjacencyMatrix;
	std::vector<std::string> IdElements;
	std::vector<int> Distance;
	std::string startID;
};
