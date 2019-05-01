//
// Created by Andrey on 20.04.2019.
//

#ifndef FOXMAPNATIVE_1_MAPITEM_H
#define FOXMAPNATIVE_1_MAPITEM_H
#include "Visitor.h"
#include <string.h>

class MapItem {
protected:
    //float vertices_[2*6];  //from top-left corner clockwise
    //float color_[4];     //RGBA
	float* vertices_;
	float color_[4];
	size_t vertices_count_;
    bool is_active_;
    bool is_clickable;
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

public:
	MapItem() {};
	MapItem(float* verts, size_t len) {
		for (int i = 0; i < (len < 12 ? len : 12); i++) {
			vertices_[i] = verts[i];
		}
	}
	virtual void Accept(Visitor&) {}
	float* GetVertices() {
		return vertices_;
	}
	size_t GetSize() {
		return vertices_count_;
	}
};

class Room : public MapItem {
public:
	Room(const float* vertices);
    void Accept(Visitor& v) override {
        v.visit(*this);
    }
	static size_t GetLength() {
		return 12;
	}
private:
};

class Passage : public MapItem {
    void Accept(Visitor& v) override {
        v.visit(*this);
    }
};

class Steps : public MapItem {
    void Accept(Visitor& v) override {
        v.visit(*this);
    }
};

#endif //FOXMAPNATIVE_1_MAPITEM_H
