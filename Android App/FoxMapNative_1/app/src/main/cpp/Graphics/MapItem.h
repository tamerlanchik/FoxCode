//
// Created by Andrey on 20.04.2019.
//

#ifndef FOXMAPNATIVE_1_MAPITEM_H
#define FOXMAPNATIVE_1_MAPITEM_H
#include "Visitor.h"
#include <vector>
#include <string>
#include "FoxUtilites/Point.h"
#include <cmath>
#include <functional>
namespace gls {
	class MapItem {
	private:
	protected:
		std::vector<float> vertices_;
		size_t vertices_count_;

	public:
		MapItem() {};

		MapItem(const std::vector<float> &verts) {
			vertices_.reserve(verts.size());
			std::copy(verts.begin(), verts.end(), std::back_inserter(vertices_));
			vertices_count_ = verts.size() / 2;
		}

		MapItem(Point &top_left, Point &bottom_right) {
			vertices_.reserve(4);
			vertices_.push_back(top_left.x);
			vertices_.push_back(top_left.y);
			vertices_.push_back(bottom_right.x);
			vertices_.push_back(bottom_right.y);
		}

		const float *GetVerticesArray() {
			return &vertices_[0];
		}

		const std::vector<float> &GetVertices() {
			return vertices_;
		}

		size_t GetSize() {
			return vertices_count_;
		}

		double GetMaxValue() const {
			return *std::max_element(vertices_.begin(), vertices_.end());
		}
	};

	class Room : public MapItem {
	public:
		Room(Point &top_left, Point &bottom_right) : MapItem(top_left, bottom_right) {
			size += 4;
			++count;
		}
		Room(Point top_left, Point bottom_right, Point entry) : MapItem(top_left, bottom_right) {
			entry_ = entry;
			size += 4;
			++count;
		}
		Room(const std::vector<float> &verts) : MapItem(verts) {
			size += verts.size();
			++count;
		};

		size_t GetLength() {
			return vertices_.size();
		}

		Point GetEntry(){
		    return entry_;
		}

		static size_t GetSize() {
			return size;
		}

		static size_t GetCount() {
			return count;
		}

	protected:
		std::string title_;
		Point entry_;
	private:
		static size_t size;
		static size_t count;
	};

	class Passage : public MapItem {
	public:
		Passage() {};
        Passage(Point top_left, Point bottom_right) : MapItem(top_left, bottom_right) {
            size += 4;
            ++count_;
        }
		Passage(std::vector<float> &vertices) : MapItem(vertices) {
			size += vertices.size();
			++count_;
		}
		static size_t GetSize() {
			return size;
		}
		static size_t GetCount() {
        	return count_;
        }

		static void ConvertRect2Lines(std::vector<Point>&);
        static std::vector<Point> GeneratePlaceholders(const std::vector<Passage>&);

        Point Center() const {
        	return Point(vertices_[2]-vertices_[0], vertices_[1]-vertices_[3]);
        }
	private:
		static size_t size;
        static size_t count_;
	};

	class Steps : public Passage {
	public:
		Steps(Point top_left, Point bottom_right) : Passage(top_left, bottom_right) {
			size += 4;
			++count_;
		}
	private:
		static size_t size;
		static size_t count_;

	};

	class Lift : public Passage {
	public:
		Lift(Point top_left, Point bottom_right) : Passage(top_left, bottom_right) {
			size += 4;
			++count_;
		}
	private:
		static size_t size;
		static size_t count_;
	};
}

#endif //FOXMAPNATIVE_1_MAPITEM_H
