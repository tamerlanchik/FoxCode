#include "pch.h"
#include "CMatrixGraph.h"


void CMatrixGraph::ptintMatrix(){
	for (int i = 0; i < IdElements.size(); i++)
		std::cout << IdElements[i] << " ";
	std::cout<<std::endl;
	for (int i = 0; i < adjacencyMatrix.size(); i++) {
		for (int j = 0; j < adjacencyMatrix.size(); j++) {
			std::cout.width(8);
			std::cout << adjacencyMatrix[i][j];
		}
		std::cout << std::endl;
	}
}

int GetHallIndex(std::vector<Hall> Halls, std::string HallID) {
	for (int i = 0; i < Halls.size(); i++)
		if (HallID == Halls[i].ID)
			return i;
}

int HallRoomDistance(Room room, Hall hall) {
	Coordinate CenterHall;
	CenterHall.x = abs(hall.RightBottom.x - hall.LeftTop.x) / 2;
	CenterHall.y = abs(hall.LeftTop.y - hall.RightBottom.y) / 2;
	int MinDistance = sqrt(pow((room.Input[0].x - CenterHall.x), 2) + pow((room.Input[0].y - CenterHall.y), 2));
	for (int i = 1; i < room.Input.size(); i++) {
		int newDistance = sqrt(pow((room.Input[i].x - CenterHall.x), 2) + pow((room.Input[i].y - CenterHall.y), 2));
		if (newDistance < MinDistance)
			MinDistance = newDistance;
	}
	return MinDistance;
}

int HallHallDistance(Hall hall1, Hall hall2) {
	Coordinate CenterHall1, CenterHall2;
	CenterHall1.x = abs(hall1.RightBottom.x - hall1.LeftTop.x) / 2;
	CenterHall1.y = abs(hall1.LeftTop.y - hall1.RightBottom.y) / 2;
	CenterHall2.x = abs(hall2.RightBottom.x - hall2.LeftTop.x) / 2;
	CenterHall2.y = abs(hall2.LeftTop.y - hall2.RightBottom.y) / 2;
	int MinDistance = sqrt(pow((CenterHall2.x - CenterHall1.x), 2) + pow((CenterHall2.y - CenterHall1.y), 2));
	return MinDistance;
}


CMatrixGraph::CMatrixGraph(const std::vector<Hall> &Halls, const std::vector<Room> &Rooms) {
	int MatrixSize = Halls.size() + Rooms.size();
	adjacencyMatrix.resize(MatrixSize);
	for (int i = 0; i < MatrixSize; i++) {
		adjacencyMatrix[i].resize(MatrixSize, 0);
	}
	for (int j = 0; j < Rooms.size(); j++) {
		IdElements.push_back(Rooms[j].ID);
		for (int k = 0; k < Rooms[j].HallID.size(); k++) {
			int HallIndex = GetHallIndex(Halls, Rooms[j].HallID[k]) + Rooms.size();
			adjacencyMatrix[j][HallIndex] = HallRoomDistance(Rooms[j], Halls[GetHallIndex(Halls, Rooms[j].HallID[k])]);
			adjacencyMatrix[HallIndex][j] = HallRoomDistance(Rooms[j], Halls[GetHallIndex(Halls, Rooms[j].HallID[k])]);
		}
	}
	for (int i = 0; i < Halls.size(); i++) {
		IdElements.push_back(Halls[i].ID);
		for (int j = 0; j < Halls[i].HallID.size(); j++) {
			int HallIndex = GetHallIndex(Halls, Halls[i].HallID[j]) + Rooms.size();
			std::cout << i + Rooms.size() << " " << HallIndex << " " << HallHallDistance(Halls[i], Halls[j]) << std::endl;
			adjacencyMatrix[i+Rooms.size()][HallIndex] = HallHallDistance(Halls[i], Halls[j]);
		}
	}
}

CMatrixGraph::CMatrixGraph(int count) {
	adjacencyMatrix.resize(count);
	for (int i = 0; i < count; i++)
		adjacencyMatrix[i].resize(count, 0);
}

CMatrixGraph::CMatrixGraph(const IGraph& graph) {
	adjacencyMatrix.resize(graph.VerticesCount());
	for (int i = 0; i < graph.VerticesCount(); ++i) {
		adjacencyMatrix[i].resize(graph.VerticesCount(), 0);
		for (int j = 0; j < graph.GetNextVertices(i).size(); j++) {
			adjacencyMatrix[i][graph.GetNextVertices(i)[j]] = 1;
		}
	}
}

CMatrixGraph::~CMatrixGraph() {}

void CMatrixGraph::AddEdge(int from, int to) {
	adjacencyMatrix[from][to] = 1;
}

int CMatrixGraph::VerticesCount() const {
	return adjacencyMatrix.size();
}

std::vector<int> CMatrixGraph::GetNextVertices(int vertex) const {
	std::vector<int> next;
	for (int i = 0; i < adjacencyMatrix[vertex].size(); ++i) {
		if (adjacencyMatrix[vertex][i] != 0)
			next.push_back(i);
	}
	return next;
}

std::vector<int> CMatrixGraph::GetPrevVertices(int vertex) const {
	std::vector<int> prev;
	for (int i = 0; i < adjacencyMatrix[vertex].size(); ++i) {
		if (adjacencyMatrix[i][vertex] != 0)
			prev.push_back(i);
	}
	return prev;
}

