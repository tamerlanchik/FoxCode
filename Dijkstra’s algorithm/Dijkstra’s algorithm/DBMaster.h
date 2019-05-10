#pragma once
#include "Entity.h"

class DBMaster{
public:
	DBMaster(std::string folder, std::string dbname, std::string* SQLQuery);
	DBMaster(std::string connectionString, std::string* SQLQuery);
	DBMaster(std::string connectionString);
	~DBMaster();
	int ReadAllData();
	int ReadHalls();
	int ReadRooms();
	int WriteAllData(struct Hall* halls, int hallsNumber, struct Room* rooms, int roomsNumber, struct MoveObject* moveObjects, int moveObjectsNumber);
	bool WriteHalls(struct Hall* halls, int hallsNumber);
	bool WriteRooms(struct Room* rooms, int roomsNumber);
	const std::vector<Hall> GetHalls();
	const std::vector<Room> GetRooms();
private:
	//int CallbackHall(void *data, int argc, char **argv, char **azColName);
	int CallbackRoom(void *data, int argc, char **argv, char **azColName);
	int CallbackDoor(void *data, int argc, char **argv, char **azColName);
	int CallbackRoomHall(void *data, int argc, char **argv, char **azColName);
	std::vector<Hall> Halls;
	std::vector<Room> Rooms;

	std::string ConnectionString;
	std::string ReadHallsSQLQuery;
	std::string ReadRoomsSQLQuery;
	std::string ReadLift;
};

