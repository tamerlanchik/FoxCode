#include "pch.h"
#include "DBMaster.h"


DBMaster::DBMaster(std::string folder, std::string dbname, std::vector<std::string> SQLQuery) {
	ConnectionString = folder + "/" + dbname;
	if (SQLQuery.size() >= 1)
		ReadHallsSQLQuery = SQLQuery[0];
	if (SQLQuery.size() >= 2)
		ReadRoomsSQLQuery = SQLQuery[1];
	if (SQLQuery.size() >= 3)
		ReadInfoSQLQuery = SQLQuery[2];
}

DBMaster::DBMaster(std::string connectionString, std::vector<std::string> SQLQuery) {
	ConnectionString = connectionString;
	if (SQLQuery.size() >= 1)
		ReadHallsSQLQuery = SQLQuery[0];
	if (SQLQuery.size() >= 2)
		ReadRoomsSQLQuery = SQLQuery[1];
	if (SQLQuery.size() >= 3)
		ReadInfoSQLQuery = SQLQuery[2];
}

DBMaster::DBMaster(std::string connectionString) {
	ConnectionString = connectionString;
	ReadHallsSQLQuery = "select hall.Id, hall.LeftTopX, Hall.LeftTopY, Hall.RightBottomX, Hall.RightBottomY, Hall.Floor,  Hall.Status from hall";
	ReadRoomsSQLQuery = "select Room.Id, Room.LeftTopX, Room.LeftTopY, Room.RightBottomX, Room.RightBottomY, Room.Floor,  Room.Status, Room.Type  from Room";
	ReadInfoSQLQuery = "select Info.Version, Info.CreationDate from Info";
}

DBMaster::DBMaster::~DBMaster()
{
}

int DBMaster::CallbackHall(void *data, int argc, char **argv, char **azColName) {
	std::vector<Hall> *Halls = static_cast<std::vector<Hall>*>(data);
	Hall TempHall;
	TempHall.ID = argv[0];
	TempHall.LeftTop.x = atoi(argv[1]);
	TempHall.LeftTop.y = atoi(argv[2]);
	TempHall.RightBottom.x = atoi(argv[3]);
	TempHall.RightBottom.y = atoi(argv[4]);
	TempHall.LeftTop.z = atoi(argv[5]);
	TempHall.RightBottom.z = atoi(argv[5]);
	if (std::string(argv[6]).size() == 4)
		TempHall.Status = true;
	else
		TempHall.Status = false;
	Halls->push_back(TempHall);
	return 0;
}

int DBMaster::GetHallIndex(std::vector<Hall> Halls, std::string HallID) {
	for (int i = 0; i < Halls.size(); i++)
		if (HallID == Halls[i].ID)
			return i;
}

int DBMaster::CallbackHallHall(void *data, int argc, char **argv, char **azColName) {
	//HallID, HallId
	std::vector<Hall> *Halls = static_cast<std::vector<Hall>*>(data);
	//Запись Id коридоров, в которых находятся аудитории
	Halls[0][GetHallIndex(Halls[0], argv[0])].HallID.push_back(argv[1]);
	Halls[0][GetHallIndex(Halls[0], argv[1])].HallID.push_back(argv[0]);
	return 0;
}

int DBMaster::CallbackRoom(void *data, int argc, char **argv, char **azColName) {
	std::vector<Room> *Rooms = static_cast<std::vector<Room>*>(data);
	Room TempRoom;
	TempRoom.ID = argv[0];
	TempRoom.LeftTop.x = atoi(argv[1]);
	TempRoom.LeftTop.y = atoi(argv[2]);
	TempRoom.RightBottom.x = atoi(argv[3]);
	TempRoom.RightBottom.y = atoi(argv[4]);
	TempRoom.LeftTop.z = atoi(argv[5]);
	TempRoom.RightBottom.z = atoi(argv[5]);
	TempRoom.Status = atoi(argv[6]);
	TempRoom.Type = std::string(argv[7]);
	Rooms->push_back(TempRoom);
	return 0;
}

int DBMaster::GetRoomIndex(std::vector<Room> Rooms, std::string RoomID) {
	for (int i = 0; i < Rooms.size(); i++)
		if (RoomID == Rooms[i].ID)
			return i;
}

int DBMaster::CallbackDoor(void *data, int argc, char **argv, char **azColName) {
	//ID, X, Y, Z, Weight
	std::vector<Room> *Rooms = static_cast<std::vector<Room>*>(data);
	Coordinate TempCoordinate;
	TempCoordinate.x = atoi(argv[1]);
	TempCoordinate.y = atoi(argv[2]);
	TempCoordinate.z = atoi(argv[3]);
	//Запись ширины дверного проёма в вектор 
	Rooms[0][GetRoomIndex(Rooms[0], argv[0])].Wight.push_back(atoi(argv[4]));
	Rooms[0][GetRoomIndex(Rooms[0], argv[0])].Input.push_back(TempCoordinate);
	return 0;
}

int DBMaster::CallbackRoomHall(void *data, int argc, char **argv, char **azColName) {
	//RoomID, HallId
	std::vector<Room> *Rooms = static_cast<std::vector<Room>*>(data);
	//Запись Id коридоров, в которых находятся аудитории
	Rooms[0][GetRoomIndex(Rooms[0], argv[0])].HallID.push_back(argv[1]);
	return 0;
}

int DBMaster::CallbackInfo(void *data, int argc, char **argv, char **azColName) {
	//HallID, HallId
	Info *Information = static_cast<Info*>(data);
	//Запись Id коридоров, в которых находятся аудитории
	Information->Version = argv[0];
	Information->CreationDate = argv[1];
	return 0;
}

int DBMaster::ReadHalls() {
	sqlite3 *MapDB = 0; // хэндл объекта соединение к БД
	char *err = 0;
	// открываем соединение
	if (sqlite3_open(ConnectionString.c_str(), &MapDB))
		return -1;
	// выполняем _SQLquery
	if (sqlite3_exec(MapDB, ReadHallsSQLQuery.c_str(), CallbackHall, (void*)&Halls, &err)) {
		return -2;
		sqlite3_free(err);
	}
	for (Hall hall : Halls) {
		std::string CurrentId = "'";
		CurrentId += hall.ID;
		CurrentId += "'";
		std::string SQLQuery = "select hallandhall.Hall1, HallAndHall.Hall2 from hallandhall where hallandhall.hall1= ";
		SQLQuery += CurrentId;
		if (sqlite3_exec(MapDB, SQLQuery.c_str(), CallbackHallHall, (void*)&Halls, &err)) {
			return -2;
			sqlite3_free(err);
		}

	}
	// закрываем соединение
	sqlite3_close(MapDB);
	return 0;
}

int DBMaster::ReadRooms() {
	sqlite3 *MapDB = 0; // хэндл объекта соединение к БД
	char *err = 0;
	// открываем соединение
	if (sqlite3_open(ConnectionString.c_str(), &MapDB))
		return -1;
	if (sqlite3_exec(MapDB, ReadRoomsSQLQuery.c_str(), CallbackRoom, (void*)&Rooms, &err)) {
		return -2; //??????????????????
		sqlite3_free(err);
	}

	for (Room room : Rooms) {
		std::string CurrentId = "'";
		CurrentId += room.ID;
		CurrentId += "'";
		std::string SQLQuery = "select RoomAndDoor.Room, Door.X, Door.Y, Door.Z, Door.Wight from Door, RoomAndDoor where ";
		SQLQuery += CurrentId;
		SQLQuery += " = RoomAndDoor.Room and Door.Id = RoomAndDoor.Door";
		if (sqlite3_exec(MapDB, SQLQuery.c_str(), CallbackDoor, (void*)&Rooms, &err)) {
			return -2;
			sqlite3_free(err);
		}
		SQLQuery = "select hallandroom.Room, HallAndRoom.Hall from hallandroom where hallandroom.Room= ";
		SQLQuery += CurrentId;
		if (sqlite3_exec(MapDB, SQLQuery.c_str(), CallbackRoomHall, (void*)&Rooms, &err)) {
			return -2;
			sqlite3_free(err);
		}

	}
	// закрываем соединение
	sqlite3_close(MapDB);
	return 0;
}

int DBMaster::ReadInfo() {
	sqlite3 *MapDB = 0; // хэндл объекта соединение к БД
	char *err = 0;
	// открываем соединение
	if (sqlite3_open(ConnectionString.c_str(), &MapDB))
		return -1;
	// выполняем _SQLquery
	if (sqlite3_exec(MapDB, ReadInfoSQLQuery.c_str(), CallbackInfo, (void*)&Information, &err)) {
		return -2;
		sqlite3_free(err);
	}
	// закрываем соединение
	sqlite3_close(MapDB);
	return 0;
}

int DBMaster::ReadAllData() {
	switch (ReadHalls()) {
	case -1:
		std::cout << "Connection Error" << std::endl;
		break;
	case -2:
		std::cout << "SQLQuery Error" << std::endl;
		break;
	default:
		std::cout << "Halls data has been read" << std::endl;
		break;
	}
	switch (ReadRooms()) {
	case -1:
		std::cout << "Connection Error" << std::endl;
		break;
	case -2:
		std::cout << "SQLQuery Error" << std::endl;
		break;
	default:
		std::cout << "Rooms data has been read" << std::endl;
		break;
	}
	switch (ReadInfo()) {
	case -1:
		std::cout << "Connection Error" << std::endl;
		break;
	case -2:
		std::cout << "SQLQuery Error" << std::endl;
		break;
	default:
		std::cout << "Info data has been read" << std::endl;
		break;
	}
	return 0;
}

const std::vector<Hall> &DBMaster::GetHalls() {
	return Halls;
}

const std::vector<Room> &DBMaster::GetRooms() {
	return Rooms;
}

const Info &DBMaster::GetInfo() {
	return Information;
}
