// Dijkstra’s algorithm.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
#include <iostream>
//#include "DijkstrasAlgorithm.h"
#include "DBMaster.h"
#include "CMatrixGraph.h"
#include <queue>

void InputDB(std::string ConnectionString) {
	int RecordNumber;
	int Floor = 3;
	std::string TempID;
	double  TempLeftTopX, TempLeftTopY, TRASH, TempRightBottomX, TempRightBottomY, TempRightBottomZ, DoorX, DoorY;
	std::vector<Room> TempRoomVector;
	std::vector<Hall> TempHallVector;
	Coordinate TempCoord;
	Room TempRoom;
	Hall TempHall;
	std::cout << "Input Number of records" << std::endl;
	std::cin >> RecordNumber;
	std::cout << "Input All records" << std::endl;
	for (int i = 0; i < RecordNumber; i++) {
		std::cin >> TempID;
		std::cin >> TRASH;
		std::cin >> TempLeftTopX;
		std::cin >> TempLeftTopY;
		std::cin >> TempRightBottomX;
		std::cin >> TempRightBottomY;
		if (TempID[0] == 'R') {
			std::cin >> DoorX;
			std::cin >> DoorY;

			TempCoord.x = (int)DoorX;
			TempCoord.y = (int)DoorY;
			TempCoord.z = Floor;


			TempRoom.ID = TempID;
			TempRoom.Type = "Room";
			TempRoom.LeftTop.x = (int)TempLeftTopX;
			TempRoom.LeftTop.y = (int)TempLeftTopY;
			TempRoom.LeftTop.z = Floor;
			TempRoom.RightBottom.x = (int)TempRightBottomX;
			TempRoom.RightBottom.y = (int)TempRightBottomY;
			TempRoom.RightBottom.z = Floor;
			TempRoom.Status = true;
			TempRoom.Input.push_back(TempCoord);
			TempRoom.Wight.push_back(1);
			TempRoomVector.push_back(TempRoom);
		}

		if (TempID[0] == 'P') {
			TempHall.ID = TempID;
			TempHall.LeftTop.x = (int)TempLeftTopX;
			TempHall.LeftTop.y = (int)TempLeftTopY;
			TempHall.LeftTop.z = Floor;
			TempHall.RightBottom.x = (int)TempRightBottomX;
			TempHall.RightBottom.y = (int)TempRightBottomY;
			TempHall.RightBottom.z = Floor;
			TempHall.Status = true;
			TempHallVector.push_back(TempHall);
		}

		if (TempID[0] == 'S') {
			TempRoom.ID = TempID;
			TempRoom.Type = "Step";
			TempRoom.LeftTop.x = (int)TempLeftTopX;
			TempRoom.LeftTop.y = (int)TempLeftTopY;
			TempRoom.LeftTop.z = Floor;
			TempRoom.RightBottom.x = (int)TempRightBottomX;
			TempRoom.RightBottom.y = (int)TempRightBottomY;
			TempRoom.RightBottom.z = Floor;
			TempRoom.Status = true;
			TempRoomVector.push_back(TempRoom);
		}

		if (TempID[0] == 'L') {
			TempRoom.ID = TempID;
			TempRoom.Type = "Lift";
			TempRoom.LeftTop.x = (int)TempLeftTopX;
			TempRoom.LeftTop.y = (int)TempLeftTopY;
			TempRoom.LeftTop.z = Floor;
			TempRoom.RightBottom.x = (int)TempRightBottomX;
			TempRoom.RightBottom.y = (int)TempRightBottomY;
			TempRoom.RightBottom.z = Floor;
			TempRoom.Status = true;
			TempRoomVector.push_back(TempRoom);
		}

	}
	for (int i = 0; i < TempRoomVector.size(); i++) {
		std::cout << TempRoomVector[i].ID << " ";
		std::cout << TempRoomVector[i].LeftTop.x << " ";
		std::cout << TempRoomVector[i].LeftTop.y << " ";
		std::cout << TempRoomVector[i].RightBottom.x << " ";
		std::cout << TempRoomVector[i].RightBottom.y << " ";
		std::cout << TempRoomVector[i].LeftTop.z << " ";
		std::cout << std::endl;
	}
	DBMaster dbMaster(ConnectionString);
	dbMaster.WriteHalls(TempHallVector);
	dbMaster.WriteRooms(TempRoomVector);
}

void BFS(const CMatrixGraph& graph, int vertex, void(*visit)(int, CMatrixGraph)) {
	std::vector<bool> visited(graph.VerticesCount(), false);
	// Тут храним текуший фронт - ?
	std::queue<int> qu;
	// Кладем вершину, с которой наиснаем обход
	qu.push(vertex);
	visited[vertex] = true;

	while (qu.size() != 0) {
		int current = qu.front();
		qu.pop();
		visit(current, graph);
		std::vector<int> adjacentVertices = graph.GetNextVertices(current);
		for (int i = 0; i < adjacentVertices.size(); ++i) {
			if (!visited[adjacentVertices[i]]) {
				qu.push(adjacentVertices[i]);
				visited[adjacentVertices[i]] = true;
			}
		}
	}
}

int main() {
	InputDB("sqlite_lib/MapDB.db");
	/*
	DBMaster dbMaster("sqlite_lib/MapDB.db");
	dbMaster.ReadAllData();
	std::cout << dbMaster.GetInfo().Version << std::endl;
	for (int i = 0; i < dbMaster.GetHalls().size(); i++) {
		std::cout << dbMaster.GetHalls()[i].ID << " ";
		for (int j = 0; j < dbMaster.GetHalls()[i].HallID.size(); j++) {
			std::cout << dbMaster.GetHalls()[i].HallID[j] << std::endl;
		}
	}
	for (int i = 0; i < dbMaster.GetRooms().size(); i++) {
		for (int j = 0; j < dbMaster.GetRooms()[i].Input.size(); j++) {
			std::cout << "X=" << dbMaster.GetRooms()[i].Input[j].x << " " << std::endl;
		}
	}

	CMatrixGraph MatrixGraph(dbMaster.GetHalls(), dbMaster.GetRooms());
	MatrixGraph.ptintMatrix();
	MatrixGraph.FindRoute("Room_501", "Room_507");
	std::vector<std::string> Root = MatrixGraph.GetLastRoute();
	for (int i = 0; i < Root.size(); i++) {
		std::cout << Root[i] << " ";
	}
	*/
	return 0;
	//(sqlite3_exec(db, _SQLquery, 0, 0, &err))
	//for (int i = 0; i < MatrixGraph.Dijkstra(1).size(); i++) {
	//	std::cout << MatrixGraph.Dijkstra(1)[i] << std::endl;
	//}

	//std::cout << MatrixGraph.IdElements.size() << std::endl;
	//MatrixGraph.ptintMatrix();
	//BFS(MatrixGraph, 0, [](int vertex, CMatrixGraph graph) { std::cout << graph.IdElements[vertex] << " "; });
	//MatrixGraph.ptintMatrix();
	//std::vector<Room> Halls = dbMaster.GetRooms();
	//DijkstrasAlgorithm<int> NewRoute(startRoom, endRoom, Matrix);
	//std::cout << Halls.size(); 
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
