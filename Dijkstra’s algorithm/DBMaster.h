#pragma once
#include "Entity.h"

class DBMaster
{
public:
	DBMaster(std::string folder, std::string dbname, std::string* SQLQuery);
	DBMaster(std::string connectionString, std::string* SQLQuery);
	~DBMaster();
	bool ReadAllData();
	int WriteAllData(struct Hall* halls, int hallsNumber, struct Room* rooms, int roomsNumber, struct MoveObject* moveObjects, int moveObjectsNumber);
	bool WriteHalls(struct Hall* halls, int hallsNumber);
	bool WriteRooms(struct Room* rooms, int roomsNumber);
	bool WriteMoveObjects(struct MoveObject* moveObjects, int moveObjectsNumber);
	const struct Hall* GetHalls();
	const int GetHallsNumber();
	const struct Room* GetRooms();
	const int GetRoomsNumber();
	const struct MoveObject* GetMoveObjects();
	const int GetMoveObjectsNumber();
private:
	struct Hall* Halls;
	int HallsNumber;
	struct Room* Rooms;
	int RoomsNumber;
	struct MoveObject* MoveObjects;
	int MoveObjectsNumber;
	std::string ConnectionString;
	std::string ReadHallsSQLQuery;
	std::string ReadRoomsSQLQuery;
	std::string ReadMoveObjectsSQLQuery;
};

