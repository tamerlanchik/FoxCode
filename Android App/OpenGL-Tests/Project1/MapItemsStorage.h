//
// Created by Andrey on 21.04.2019.
//

#ifndef FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
#define FOXMAPNATIVE_1_MAPITEMSSTORAGE_H

#include <vector>
#include "MapItem.h"
#include "DataBase.h"
#include "Point.h"
#include "Log.h"
#include <system_error>
using std::vector;

class MapItemStorage {
protected:
    vector<MapItem*> storage_;
	vector<float> buffer_;
	vector<Room*> room_storage_;
	vector<Passage*> passage_storage_;
	size_t rooms_buf_size_;
	size_t passages_buf_size_;
	DataBase* database_;
	MapItemStorage();
public:
	static MapItemStorage* Get();

	void SetDatabase(DataBase*);

	virtual bool InflateStorage();
private:
	static const char TAG[];
};

#endif //FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
