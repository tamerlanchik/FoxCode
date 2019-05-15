#include "pch.h"
#include "DBMaster.h"
#include <algorithm>
#include "sqlite_lib/sqlite3.h"

DBMaster::DBMaster(std::string folder, std::string dbname, std::string* SQLQuery) {
	ConnectionString = folder + "/" + dbname;
	if (SQLQuery != nullptr)
		ReadHallsSQLQuery = SQLQuery[0];
	else
		ReadHallsSQLQuery = "";
	if (SQLQuery + 1 != nullptr)
		ReadRoomsSQLQuery = SQLQuery[1];
	else
		ReadRoomsSQLQuery = "";
}

DBMaster::DBMaster(std::string connectionString, std::string* SQLQuery) {
	ConnectionString = connectionString;
	if (SQLQuery != nullptr)
		ReadHallsSQLQuery = SQLQuery[0];
	else
		ReadHallsSQLQuery = "";
	if (SQLQuery + 1 != nullptr)
		ReadRoomsSQLQuery = SQLQuery[1];
	else
		ReadRoomsSQLQuery = "";
}

DBMaster::DBMaster(std::string connectionString) {
	ConnectionString = connectionString;
	ReadHallsSQLQuery = "select hall.Id, hall.LeftTopX, Hall.LeftTopY, Hall.RightBottomX, Hall.RightBottomY, Hall.Floor,  Hall.Status from hall";
	ReadRoomsSQLQuery = "select Room.Id, Room.LeftTopX, Room.LeftTopY, Room.RightBottomX, Room.RightBottomY, Room.Floor,  Room.Status, Room.Type  from Room";
}

DBMaster::DBMaster::~DBMaster()
{
}

int CallbackHall(void *data, int argc, char **argv, char **azColName) {
	std::vector<Hall> *Halls = static_cast<std::vector<Hall>*>(data);
	Hall TempHall;
	TempHall.ID = atoi(argv[0]);
	TempHall.LeftTop.x = atoi(argv[1]);
	TempHall.LeftTop.y = atoi(argv[2]);
	TempHall.RightBottom.x = atoi(argv[3]);
	TempHall.RightBottom.y = atoi(argv[4]);
	TempHall.LeftTop.z = atoi(argv[5]);
	TempHall.RightBottom.z = atoi(argv[5]);
	TempHall.Status = atoi(argv[6]);
	Halls->push_back(TempHall);
	return 0;
}

int CallbackRoom(void *data, int argc, char **argv, char **azColName) {
	std::vector<Room> *Rooms = static_cast<std::vector<Room>*>(data);
	Room TempRoom;
	TempRoom.ID = atoi(argv[0]);
	TempRoom.LeftTop.x = atoi(argv[1]);
	TempRoom.LeftTop.y = atoi(argv[2]);
	TempRoom.RightBottom.x = atoi(argv[3]);
	TempRoom.RightBottom.y = atoi(argv[4]);
	TempRoom.LeftTop.z = atoi(argv[5]);
	TempRoom.RightBottom.z = atoi(argv[5]);
	TempRoom.Status = atoi(argv[6]);
	TempRoom.Type = std::string(argv[6]);
	Rooms->push_back(TempRoom);
	return 0;
}

int CallbackDoor(void *data, int argc, char **argv, char **azColName) {
	//ID, X, Y, Z, Weight
	std::vector<Room> *Rooms = static_cast<std::vector<Room>*>(data);
	Coordinate TempCoordinate;
	TempCoordinate.x = atoi(argv[1]);
	TempCoordinate.y = atoi(argv[2]);
	TempCoordinate.z = atoi(argv[3]);
	//Запись ширины дверного проёма в вектор 
	Rooms[0][atoi(argv[0])].Wight.push_back(atoi(argv[4]));
	Rooms[0][atoi(argv[0])].Input.push_back(TempCoordinate);
	return 0;
}

int CallbackRoomHall(void *data, int argc, char **argv, char **azColName) {
	//RoomID, HallId
	std::vector<Room> *Rooms = static_cast<std::vector<Room>*>(data);
	//Запись Id коридоров, в которых находятся аудитории
	Rooms[0][atoi(argv[0])].HallID.push_back(atoi(argv[1]));
	return 0;
}

int DBMaster::ReadHalls() {
	sqlite3 *MapDB = 0; // хэндл объекта соединение к БД
	char *err = 0;
	Hall *DataHall = nullptr;
	// открываем соединение
	if (sqlite3_open(ConnectionString.c_str(), &MapDB))
		return -1;//fprintf(stderr, "Error create DB : %s\n", sqlite3_errmsg(MapDB));
	// выполняем _SQLquery
	if (sqlite3_exec(MapDB, ReadHallsSQLQuery.c_str(), CallbackHall, (void*)&Halls, &err)) {
		return -1;//fprintf(stderr, "Error SQL query : %sn", err);
		sqlite3_free(err);
	}
	// закрываем соединение
	sqlite3_close(MapDB);
	return 0;
}

int DBMaster::ReadRooms() {
	sqlite3 *MapDB = 0; // хэндл объекта соединение к БД
	char *err = 0;
	const char* data = nullptr;
	// открываем соединение
	if (sqlite3_open("MapDB.db", &MapDB))
		return -1;//fprintf(stderr, "Error create DB : %s\n", sqlite3_errmsg(MapDB));
	if (sqlite3_exec(MapDB, ReadRoomsSQLQuery.c_str(), CallbackRoom, (void*)&Rooms, &err)) {
		return -1; //??????????????????
		sqlite3_free(err);
	}

	for (Room room : Rooms) {
		int CurrentId = room.ID;
		//select Room.Id, Door.Id, Door.X, Door.Y, Door.Z, Door.Wight from Door, Room, RoomAndDoor where CURRENTID = RoomAndDoor.Room and RoomAndDoor.Door = CURRENTID
		std::string SQLQuery = "select Room.Id, Door.Id, Door.X, Door.Y, Door.Z, Door.Wight from Door, Room, RoomAndDoor where";
		SQLQuery += std::string(1, CurrentId);
		SQLQuery += "= RoomAndDoor.Room and RoomAndDoor.Door =";
		SQLQuery += std::string(1, CurrentId);
		if (sqlite3_exec(MapDB, SQLQuery.c_str(), CallbackDoor, (void*)data, &err)) {
			return -1;
			sqlite3_free(err);
		}
		SQLQuery = "select Room.Id, HallAndRoom.Hall from room, hallandroom where hallandroom.Room= ";
		SQLQuery += std::string(1, CurrentId);
		if (sqlite3_exec(MapDB, SQLQuery.c_str(), CallbackRoomHall, (void*)data, &err)) {
			return -1;
			sqlite3_free(err);
		}
	}
	// закрываем соединение
	sqlite3_close(MapDB);
	return 0;
}

int DBMaster::ReadAllData() {
	sqlite3 *MapDB = 0; // хэндл объекта соединение к БД
	char *err = 0;
	const char* data = nullptr;
	// открываем соединение
	if (sqlite3_open("MapDB.db", &MapDB))
		return -1;//fprintf(stderr, "Error create DB : %s\n", sqlite3_errmsg(MapDB));
	ReadHalls();
	ReadRooms();
	// закрываем соединение
	sqlite3_close(MapDB);
	return true;
}

const std::vector<Hall> DBMaster::GetHalls() {
	return Halls;
}

const std::vector<Room> DBMaster::GetRooms() {
	return Rooms;
}
