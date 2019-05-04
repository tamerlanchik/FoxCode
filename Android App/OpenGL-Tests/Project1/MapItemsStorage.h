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
	DataBase* database_;

    //static MapItemStorage* instance_;

	MapItemStorage();
public:
    /*class Point {
        float x_;
        float y_;
    public:
        Point() : x_(0.0), y_(0.0) {};
        Point(float x, float y) : x_(x), y_(y) {};

        float GetX() { return x_; }
        float GetY() { return y_; }
        float* GetXY() {
            float* buf = new float[2];
            buf[0] = x_; buf[1] = y_;
            return buf;
        }

        void GetY(float y) { y_ = y; }
        void SetX(float x) { x_ = x; }
    };*/

	static MapItemStorage* Get();

	void SetDatabase(DataBase*);

    MapItem* GetItem(UUID id) const { return NULL; }

    MapItem* GetItem(Point& point) const { return NULL; }

    size_t Size() { return storage_.size(); }

	virtual bool InflateStorage();

	float* GetRectangles();

	float* GetRooms();
	const size_t GetBufferSize() const { return buffer_.size(); }

private:
    //void inflateItem(UUID id);
    //vector<UUID> getIdList() const;
    //DataBaseMaster::MapItem getObjectData(UUID id) const;
    //vector<DataBaseMaster::MapItem> getAllObjectData() const;
};

#endif //FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
