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
		/*float temp[12] = {  -0.7, 0.7, 
							-0.8, 0.7, 
							-0.8, 0.2,
							-0.1, 0.2, 
							-0.1, 0.7,
							-0.6, 0.7 };
		std::vector<float> t;
		Room* item = new Room(std::vector<float>(temp));
		storage_.push_back(item);
		room_storage_.push_back(item);

		float temp1[12] = { 0.79, 0.15,
							0.9, 0.25,
							0.65, 0.55,
							0.16, 0.18,
							0.44, -0.12,
							0.6, 0 };
		item = new Room(temp1);
		room_storage_.push_back(item);
		*/
		/*float temp[8] = { -1, -1, -1, -0.5, -0.5, -0.5 , -0.5, -1};
		MapItem* item = new MapItem(temp, 8);
		storage_.push_back(item);

		float temp1[8] = { -1, 1, -1, 0.5, -0.5, 0.5, -0.5, 1};
		item = new MapItem(temp1, 8);
		storage_.push_back(item);*/
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
		assert(database_);
		std::vector<DataBase::RoomParcel> data = database_->GetRooms();
		std::vector<float> vertices(4);
		for (auto item : data) {
			vertices.push_back(item.top_left_.x);
			vertices.push_back(item.top_left_.y);
			vertices.push_back(item.bottom_right.x);
			vertices.push_back(item.bottom_right.y);
			Room* room = new Room(vertices);
			room_storage_.push_back(room);
		}
		int i = 0;
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
		size_t verts_len = Room::GetLength();
		buffer_.reserve(room_storage_.size() * verts_len);
		/*float* verts;
		for (Room* i : room_storage_) {
			verts = i->GetVertices();
			std::copy(verts, verts + verts_len, std::back_inserter(buffer_));
		}*/
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
