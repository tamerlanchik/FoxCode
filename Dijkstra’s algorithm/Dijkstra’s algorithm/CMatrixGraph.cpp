#include "pch.h"
#include "CMatrixGraph.h"


void CMatrixGraph::ptintMatrix() {
	for (int i = 0; i < IdElements.size(); i++)
		std::cout << IdElements[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < adjacencyMatrix.size(); i++) {
		for (int j = 0; j < adjacencyMatrix.size(); j++) {
			std::cout.width(8);
			std::cout << adjacencyMatrix[i][j];
		}
		std::cout << std::endl;
	}
}

int CMatrixGraph::GetHallIndex(std::vector<Hall> Halls, std::string HallID) {
	for (int i = 0; i < Halls.size(); i++)
		if (HallID == Halls[i].ID)
			return i;
}

int CMatrixGraph::HallRoomDistance(Room room, Hall hall) {
	Coordinate CenterHall;
	CenterHall.x = abs(hall.RightBottom.x - hall.LeftTop.x) / 2;
	CenterHall.y = abs(hall.LeftTop.y - hall.RightBottom.y) / 2;
	if (room.Input.size() > 0) {
		int MinDistance = sqrt(pow((room.Input[0].x - CenterHall.x), 2) + pow((room.Input[0].y - CenterHall.y), 2));
		for (int i = 1; i < room.Input.size(); i++) {
			int newDistance = sqrt(pow((room.Input[i].x - CenterHall.x), 2) + pow((room.Input[i].y - CenterHall.y), 2));
			if (newDistance < MinDistance)
				MinDistance = newDistance;
		}
		return MinDistance;
	}
	else
		return 0;

}

int CMatrixGraph::HallHallDistance(Hall hall1, Hall hall2) {
	Coordinate CenterHall1, CenterHall2;
	CenterHall1.x = abs(hall1.RightBottom.x - hall1.LeftTop.x) / 2;
	CenterHall1.y = abs(hall1.LeftTop.y - hall1.RightBottom.y) / 2;
	CenterHall2.x = abs(hall2.RightBottom.x - hall2.LeftTop.x) / 2;
	CenterHall2.y = abs(hall2.LeftTop.y - hall2.RightBottom.y) / 2;
	int MinDistance = sqrt(pow((CenterHall2.x - CenterHall1.x), 2) + pow((CenterHall2.y - CenterHall1.y), 2));
	return MinDistance;
}

int CMatrixGraph::RefreshData(const std::vector<Hall> &Halls, const std::vector<Room> &Rooms) {
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
			std::cout << "Number Hall" << i + Rooms.size() << " " << HallIndex << " " << HallHallDistance(Halls[i], Halls[GetHallIndex(Halls, Halls[i].HallID[j])]) << std::endl;
			adjacencyMatrix[i + Rooms.size()][HallIndex] = HallHallDistance(Halls[i], Halls[GetHallIndex(Halls, Halls[i].HallID[j])]);
		}
	}
	return 0;
}

CMatrixGraph::CMatrixGraph(const std::vector<Hall> &Halls, const std::vector<Room> &Rooms) {
	RefreshData(Halls, Rooms);
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

std::vector<int> CMatrixGraph::Dijkstra(const std::vector<std::vector<int>> Matrix, int StartIndex) {
	std::vector<int> distance(Matrix.size(), INT_MAX);
	std::vector<bool> visited(Matrix.size(), false);
	int index, m = StartIndex;
	int Size = Matrix.size();
	distance[StartIndex] = 0;
	for (int count = 0; count < Size - 1; count++) {
		int min = INT_MAX;
		for (int i = 0; i < Size; i++)
			if (!visited[i] && distance[i] <= min) {
				min = distance[i];
				index = i;
			}
		visited[index] = true;
		for (int i = 0; i < Size; i++)
			if (!visited[i] && Matrix[index][i] && distance[index] != INT_MAX &&
				distance[index] + Matrix[index][i] < distance[i])
				distance[i] = distance[index] + Matrix[index][i];
	}
	return distance;
}

int CMatrixGraph::FindRoute(std::string StartID, std::string EndID) {
	int StartIndex = -1;
	int EndIndex = -1;
	int index = 0;
	while (index < IdElements.size() && (StartIndex == -1 || EndIndex == -1)) {
		if (IdElements[index] == StartID)
			StartIndex = index;
		if (IdElements[index] == EndID)
			EndIndex = index;
		index++;
	}
	if (StartIndex == -1) {
		Error = "Start Id not found";
		return -1;
	}
	startID = StartID;
	if (EndIndex == -1) {
		Error = "End Id not found";
		return -1;
	}
	Distance = Dijkstra(adjacencyMatrix, StartIndex);
	if (Distance[EndIndex] != INT_MAX) {
		LastRoute.push_back(IdElements[EndIndex]);
		while (EndIndex != StartIndex) {
			for (int i = 0; i < Distance.size(); i++) {
				if (adjacencyMatrix[EndIndex][i] != 0) {
					if (Distance[EndIndex] - adjacencyMatrix[EndIndex][i] == Distance[i]) {
						EndIndex = i;
						LastRoute.push_back(IdElements[i]);
					}
				}
			}
		}
	}
	return 0;
}

std::vector<std::string> CMatrixGraph::GetLastRoute() {
	return LastRoute;
}