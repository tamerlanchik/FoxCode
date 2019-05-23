// Dijkstra’s algorithm.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
#include <iostream>
//#include "DijkstrasAlgorithm.h"
#include "DBMaster.h"
#include "CMatrixGraph.h"
#include <queue>

bool PointOnLine(int StartX, int StartY, int EndX, int EndY, int PointX, int PointY) {
	if (PointY == (double(EndY - StartY)*PointX) / (double(EndX - StartX)) + double(StartY*EndX - EndY * StartX) / double(EndX - StartX))
		return true;
	else
		return false;
}

void InputDB(std::string ConnectionString) {
	int RecordNumber;
	int Floor = 4;
	std::string TempID;
	double  TempLeftTopX, TempLeftTopY, TRASH, TempRightBottomX, TempRightBottomY, TempRightBottomZ, DoorX, DoorY;
	std::vector<Room> TempRoomVector;
	std::vector<Hall> TempHallVector;
	Coordinate TempCoord;

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
			Room TempRoom;
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
			Room TempRoom;
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
			Room TempRoom;
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
	for (int i = 0; i < TempRoomVector.size(); i++) {
		for (int k = 0; k < TempHallVector.size(); k++) {
			if (TempHallVector[k].LeftTop.z == TempRoomVector[i].LeftTop.z) {
				int Xk1 = TempHallVector[k].LeftTop.x;
				int Yk1 = TempHallVector[k].LeftTop.y;
				int Xk2 = TempHallVector[k].RightBottom.x;
				int Yk2 = TempHallVector[k].RightBottom.y;
				if (TempRoomVector[i].ID[0] == 'R') {
					for (int j = 0; j < TempRoomVector[i].Input.size(); j++) {
						if (TempRoomVector[i].Input[j].x == Xk1 || TempRoomVector[i].Input[j].x == Xk2) {
							if ((TempRoomVector[i].Input[j].y <= Yk1 && TempRoomVector[i].Input[j].y >= Yk2) || (TempRoomVector[i].Input[j].y <= Yk2 && TempRoomVector[i].Input[j].y >= Yk1))
								TempRoomVector[i].HallID.push_back(TempHallVector[k].ID);
						}
						else
							if (TempRoomVector[i].Input[j].y == Yk1 || TempRoomVector[i].Input[j].y == Yk2)
								if ((TempRoomVector[i].Input[j].x <= Xk1 && TempRoomVector[i].Input[j].x >= Xk2) || (TempRoomVector[i].Input[j].x <= Xk2 && TempRoomVector[i].Input[j].x >= Xk1)) {
									TempRoomVector[i].HallID.push_back(TempHallVector[k].ID);
								}
					}
				}
				else {
					int Xi1 = TempRoomVector[i].LeftTop.x;
					int Yi1 = TempRoomVector[i].LeftTop.y;
					int Xi2 = TempRoomVector[i].RightBottom.x;
					int Yi2 = TempRoomVector[i].RightBottom.y;
					if ((Xi1 == Xk1 && Yi1 == Yk1) || (Xi1 == Xk2 && Yi1 == Yk1) || (Xi1 == Xk1 && Yi1 == Yk2) || (Xi1 == Xk2 && Yi1 == Yk2))
						TempRoomVector[i].HallID.push_back(TempHallVector[k].ID);
					if ((Xi2 == Xk1 && Yi1 == Yk1) || (Xi2 == Xk2 && Yi1 == Yk1) || (Xi2 == Xk1 && Yi1 == Yk2) || (Xi2 == Xk2 && Yi1 == Yk2))
						TempRoomVector[i].HallID.push_back(TempHallVector[k].ID);
					if ((Xi1 == Xk1 && Yi2 == Yk1) || (Xi1 == Xk2 && Yi2 == Yk1) || (Xi1 == Xk1 && Yi2 == Yk2) || (Xi1 == Xk2 && Yi2 == Yk2))
						TempRoomVector[i].HallID.push_back(TempHallVector[k].ID);
					if ((Xi2 == Xk1 && Yi2 == Yk1) || (Xi2 == Xk2 && Yi2 == Yk1) || (Xi2 == Xk1 && Yi2 == Yk2) || (Xi2 == Xk2 && Yi2 == Yk2))
						TempRoomVector[i].HallID.push_back(TempHallVector[k].ID);
				}
			}
		}
	}
	for (int i = 0; i < TempHallVector.size(); i++) {
		int Xi1 = TempHallVector[i].LeftTop.x;
		int Yi1 = TempHallVector[i].LeftTop.y;
		int Xi2 = TempHallVector[i].RightBottom.x;
		int Yi2 = TempHallVector[i].RightBottom.y;
		for (int j = i + 1; j < TempHallVector.size(); j++) {
			int Xj1 = TempHallVector[j].LeftTop.x;
			int Yj1 = TempHallVector[j].LeftTop.y;
			int Xj2 = TempHallVector[j].RightBottom.x;
			int Yj2 = TempHallVector[j].RightBottom.y;
			if (TempHallVector[i].LeftTop.z == TempHallVector[j].LeftTop.z) {
				if ((Xi1 == Xj1 && Yi1 == Yj1) || (Xi1 == Xj2 && Yi1 == Yj1) || (Xi1 == Xj1 && Yi1 == Yj2) || (Xi1 == Xj2 && Yi1 == Yj2))
					TempHallVector[i].HallID.push_back(TempHallVector[j].ID);
				if ((Xi2 == Xj1 && Yi1 == Yj1) || (Xi2 == Xj2 && Yi1 == Yj1) || (Xi2 == Xj1 && Yi1 == Yj2) || (Xi2 == Xj2 && Yi1 == Yj2))
					TempHallVector[i].HallID.push_back(TempHallVector[j].ID);
				if ((Xi1 == Xj1 && Yi2 == Yj1) || (Xi1 == Xj2 && Yi2 == Yj1) || (Xi1 == Xj1 && Yi2 == Yj2) || (Xi1 == Xj2 && Yi2 == Yj2))
					TempHallVector[i].HallID.push_back(TempHallVector[j].ID);
				if ((Xi2 == Xj1 && Yi2 == Yj1) || (Xi2 == Xj2 && Yi2 == Yj1) || (Xi2 == Xj1 && Yi2 == Yj2) || (Xi2 == Xj2 && Yi2 == Yj2))
					TempHallVector[i].HallID.push_back(TempHallVector[j].ID);
			}
		}
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
	//InputDB("sqlite_lib/MapDB.db");
	
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
	MatrixGraph.FindRoute("Room_420", "Room_420");
	std::vector<std::string> Root = MatrixGraph.GetLastRoute();
	for (int i = 0; i < Root.size(); i++) {
		std::cout << Root[i] << " ";
	}
	
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
