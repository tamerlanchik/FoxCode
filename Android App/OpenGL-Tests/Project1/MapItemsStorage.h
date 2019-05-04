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
using std::vector;
using std::cerr;
using std::cout;

class DataBaseMaster{
public:
    struct MapItem{

    };
};

class MapItemStorage {
private:
    vector<MapItem*> storage_;
	vector<float> buffer_;
	vector<Room*> room_storage_;
	DataBase* database_;

    //static MapItemStorage* instance_;

    MapItemStorage() {
        cout << "MapItemStorage created\n";
		std::cout << sizeof(MapItem) << "\n";
    };
public:
    class Point {
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
    };

    static MapItemStorage* Get() {
        static MapItemStorage instance_;
        /*if (instance_ == NULL) {
            cerr << "instance is NULL\n";
            instance_ = new MapItemStorage();
        }*/
        return &instance_;
    }

	void SetDatabase(DataBase* database) {
		database_ = database;
	}

    MapItem* GetItem(UUID id) const { return NULL; }

    MapItem* GetItem(Point& point) const { return NULL; }

    size_t Size() { return storage_.size(); }

	bool InflateStorage() {
		if (!database_)
			return 0;
		std::vector<DataBase::RoomParcel> data = database_->GetRooms();
		for (DataBase::RoomParcel c : data) {
			Room* room = new Room(c.lines);
			room_storage_.push_back(room);
		}
		return 1;
	}

	float* GetRectangles() {
		/*const size_t vertex_count = 6;
		float vertex_data_[vertex_count] = { -1, -1, -1, 1, 0, 0 };
		buffer_.reserve(vertex_count);
		for (float i : vertex_data_)
			buffer_.push_back(i);*/
		/*buffer_.reserve(storage_.size() * (2*4));
		for (MapItem* i : storage_) {
			float* verts = i->GetVertices();
			for (int j = 0; j < 8; j++)
				buffer_.push_back(verts[j]);
		}*/
		float* v = &buffer_[0];
		return v;
	}
	float* GetRooms() {
		//size_t verts_len = Room::GetLength();
		buffer_.reserve(Room::GetSize());
		std::vector<float> verts;
		for (Room* i : room_storage_) {
			verts = i->GetVertices();
			std::copy(verts.begin(), verts.end(), std::back_inserter(buffer_));
		}
		return &buffer_[0];
	}
	const size_t GetBufferSize() const { return buffer_.size(); }

private:
    void inflateItem(UUID id);
    vector<UUID> getIdList() const;
    DataBaseMaster::MapItem getObjectData(UUID id) const;
    vector<DataBaseMaster::MapItem> getAllObjectData() const;
};

#endif //FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
