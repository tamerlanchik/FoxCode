//
// Created by Andrey on 20.04.2019.
//

#ifndef FOXMAPNATIVE_1_MAPITEM_H
#define FOXMAPNATIVE_1_MAPITEM_H
#include "Visitor.h"
#include <vector>
#include <string>
#include "FoxUtilites/Point.h"

class MapItem {
private:
protected:
	std::vector<float> vertices_;
	size_t vertices_count_;

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
	MapItem() {};
	MapItem(const std::vector<float>& verts) {
		vertices_.reserve(verts.size());
		std::copy(verts.begin(), verts.end(), std::back_inserter(vertices_));
		vertices_count_ = verts.size()/2;
	}
	MapItem(Point& top_left, Point& bottom_right) {
		vertices_.reserve(8);
		vertices_.push_back(top_left.x);
		vertices_.push_back(top_left.y);
	}
	virtual void Accept(Visitor&) = 0;

	const float* GetVerticesArray() {
		return &vertices_[0];
	}
	const std::vector<float>& GetVertices() {
		return vertices_;
	}
	size_t GetSize() {
		return vertices_count_;
	}
};

class Room : public MapItem {
public:
	Room(const std::vector<float>& verts) : MapItem(verts) {
		size += verts.size();
		++count;
	};
    void Accept(Visitor& v) override {
        v.visit(*this);
    }
	size_t GetLength() {
		return vertices_.size();
	}
	static size_t GetSize() {
		return size;
	}
	static size_t GetCount() {
		return count;
	}
protected:
	std::string title_;
private:
	static size_t size;
	static size_t count;
};


class Passage : public MapItem {
public:
	Passage() {};
	Passage(std::vector<float>& vertices) : MapItem(vertices){
		size += vertices.size();
	}
    void Accept(Visitor& v) override {
        v.visit(*this);
    }
	static size_t GetSize() {
		return size;
	}
private:
	static size_t size;

};

class Steps : public MapItem {
public:
	Steps(std::vector<float>& vertices) : MapItem(vertices) {
		// добавление точек для сетки
	}
    void Accept(Visitor& v) override {
        v.visit(*this);
    }
};

#endif //FOXMAPNATIVE_1_MAPITEM_H
