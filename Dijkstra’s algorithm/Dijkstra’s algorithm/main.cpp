// Dijkstra’s algorithm.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
#include <iostream>
//#include "DijkstrasAlgorithm.h"
#include "DBMaster.h"
#include "CMatrixGraph.h"
#include <queue>
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

struct Coordinates {
	double x;
	double y;
	double z;
};

struct Halls {
	std::string ID;
	Coordinates LeftTop;
	Coordinates RightBottom;
	std::vector<std::string> HallID;
	bool Status; //Работает или нет
};

struct Rooms:Halls {
	std::vector<Coordinate> Input;
	std::vector<int> Wight; //Под вопросом, мб не будет и везде будет стандарная ширина дверного проёма
	std::string Type; //Аудитория, лифт, лестница, столовая и т. д.
};

int main() {
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
	sqlite3 *MapDB = 0; // хэндл объекта соединение к БД
	char *err = 0;
	// открываем соединение
	if (sqlite3_open("sqlite_lib/MapDB.db", &MapDB))
		return -1;
	int RecordNumber;
	Rooms TempRoom;
	std::cin >> RecordNumber;
	for (int i = 0; i < RecordNumber; i++) {
		std::cin >> TempRoom.ID;
		std::cin >> TempRoom.LeftTop.z;
		TempRoom.Type = "Audit";
		std::cin >> TempRoom.LeftTop.x;
		std::cin >> TempRoom.LeftTop.y;
		std::cin >> TempRoom.RightBottom.x;
		std::cin >> TempRoom.RightBottom.y;
		//TempRoom.LeftTop.z = 1;
		TempRoom.Status = true;
	}
	for (int i = 0; i < RecordNumber; i++) {
		std::cout << TempRoom.ID << " ";
		//TempRoom.Type = "Audit";
		std::cout << (int)TempRoom.LeftTop.x << " ";
		std::cout << TempRoom.LeftTop.y << " ";
		std::cout << TempRoom.RightBottom.x << " ";
		std::cout << TempRoom.RightBottom.y << " ";
		std::cout << TempRoom.LeftTop.z << " ";
		//TempRoom.LeftTop.z = 1;
		//TempRoom.Status = true;
		std::cout << std::endl;
	}
	sqlite3_close(MapDB);
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
