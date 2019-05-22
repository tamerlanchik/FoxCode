#pragma once
#include "Entity.h"

class DBMaster{
public:
	DBMaster(std::string folder, std::string dbname, std::vector<std::string> SQLQuery);
	DBMaster(std::string connectionString, std::vector<std::string> SQLQuery);
	DBMaster(std::string connectionString);
	~DBMaster();
	int ReadAllData();
	int ReadHalls();
	int ReadRooms();
	int ReadInfo();
	int WriteAllData(struct Hall* halls, int hallsNumber, struct Room* rooms, int roomsNumber, struct MoveObject* moveObjects, int moveObjectsNumber);
	bool WriteHalls(struct Hall* halls, int hallsNumber);
	bool WriteRooms(struct Room* rooms, int roomsNumber);
	const std::vector<Hall> &GetHalls();
	const std::vector<Room> &GetRooms();
	const Info &GetInfo();
private:
	static int CallbackHall(void *data, int argc, char **argv, char **azColName);
	static int CallbackHallHall(void *data, int argc, char **argv, char **azColName);
	static int CallbackRoom(void *data, int argc, char **argv, char **azColName);
	static int CallbackDoor(void *data, int argc, char **argv, char **azColName);
	static int CallbackRoomHall(void *data, int argc, char **argv, char **azColName);
	static int CallbackInfo(void *data, int argc, char **argv, char **azColName);
	static int GetHallIndex(std::vector<Hall> Halls, std::string HallID);
	static int GetRoomIndex(std::vector<Room> Rooms, std::string RoomID);
	std::vector<Hall> Halls;
	std::vector<Room> Rooms;
	std::string ConnectionString;
	std::string ReadHallsSQLQuery;
	std::string ReadRoomsSQLQuery;
	std::string ReadInfoSQLQuery;
	Info Information;
};

