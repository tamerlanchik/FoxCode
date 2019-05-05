//
// Created by Andrey on 21.04.2019.
//

#ifndef FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
#define FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
#define UUID unsigned int

#include "MapItem.h"
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include "DataBase.h"
#include "Point.h"
using std::vector;
using std::cerr;
using std::cout;

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
	float* getRooms();
	float* getPassages();
public:
	static MapItemStorage* Get();

	void SetDatabase(DataBase*);

    MapItem* GetItem(UUID id) const { return NULL; }

    MapItem* GetItem(Point& point) const { return NULL; }

    size_t Size() { return storage_.size(); }

	virtual bool InflateStorage();

	float* GetRectangles();

	float* GetRooms();
	float* GetPassages();
	float* GetObjects();
	size_t GetRoomsBufSize() const { return rooms_buf_size_; }
	size_t GetPassagesBufSize() const { return passages_buf_size_; }
	const size_t GetBufferSize() const { return buffer_.size(); }

private:
    //void inflateItem(UUID id);
    //vector<UUID> getIdList() const;
    //DataBaseMaster::MapItem getObjectData(UUID id) const;
    //vector<DataBaseMaster::MapItem> getAllObjectData() const;
};

#endif //FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
