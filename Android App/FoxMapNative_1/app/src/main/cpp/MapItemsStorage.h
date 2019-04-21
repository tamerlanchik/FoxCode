//
// Created by Andrey on 21.04.2019.
//

#ifndef FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
#define FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
#define UUID unsigned int

#include "MapItem.h"
#include <vector>
#include <iostream>
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
    //static MapItemStorage* instance_;

    MapItemStorage() {
        cout << "MapItemStorage created\n";
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

    MapItem* GetItem(UUID id) const { return NULL; }

    MapItem* GetItem(Point& point) const { return NULL; }

    size_t Size() { return storage_.size(); }

    bool InflateStorage();

private:
    void inflateItem(UUID id);
    vector<UUID> getIdList() const;
    DataBaseMaster::MapItem getObjectData(UUID id) const;
    vector<DataBaseMaster::MapItem> getAllObjectData() const;
};

#endif //FOXMAPNATIVE_1_MAPITEMSSTORAGE_H
