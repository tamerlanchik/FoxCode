//
// Created by Andrey on 21.04.2019.
//

#ifndef FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
#define FOXMAPNATIVE_1_MAPITEMSSTORAGE_H

#include <vector>
#include "Database/DataBase.h"
#include "FoxUtilites/Point.h"
#include "FoxUtilites/Log.h"
#include <system_error>
#include <Database/DBMaster.h>
#include "Database/Entity.h"
#include "MapItem.h"
using std::vector;

class MapItemStorage {
protected:
    //vector<std::vector<gls::MapItem*>& > storage_;
	vector<gls::Room*> room_storage_;
	vector<gls::Passage*> passage_storage_;
	vector<gls::Lift*> lift_storage_;
	std::vector<gls::Steps*> steps_storage_;
	//DataBase* database_;
	//DBMaster* db_;
	MapItemStorage();
	bool is_inflated_;
	double getMaxCoordinateValue() const;
public:
	class DBAdapter{
	public:
		virtual std::vector<Room> GetRooms() = 0;
		virtual std::vector<Hall> GetPassages() = 0;
	};
	virtual bool InflateStorage(DBAdapter&);
	static MapItemStorage* Get();

	void SetDatabase(DataBase*);
	void SetDatabase(DBMaster*);

	virtual bool InflateStorage();

	bool IsInflated() const;
private:
	static const char TAG[];
	void convertPassages();
};

#endif //FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
