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
    struct MapKeys{
        static const std::string Room;
        static const std::string Pass;
        static const std::string Lift;
        static const std::string Steps;
    };
	class MapItem {
	private:
	protected:
		std::vector<float> vertices_;
		size_t vertices_count_;
		std::string id_;

	public:
		class Compare{
		public:
			bool operator() (MapItem* const a, MapItem* const b) const{
				return a->id_ < b->id_;
			}
		};
		MapItem() {};

		MapItem(const std::vector<float> &verts) {
			vertices_.reserve(verts.size());
			std::copy(verts.begin(), verts.end(), std::back_inserter(vertices_));
			vertices_count_ = verts.size() / 2;
		}

		MapItem(Point &top_left, Point &bottom_right, std::string id) {
			vertices_.reserve(4);
			vertices_.push_back(top_left.x);
			vertices_.push_back(top_left.y);
			vertices_.push_back(bottom_right.x);
			vertices_.push_back(bottom_right.y);
			id_ = id;
		}

		const float *GetVerticesArray() const {
			return &vertices_[0];
		}
		void SetID(const std::string& id){
			id_ = id;
		}
		virtual Point GetCenter() const {
			return Point(vertices_[2]+vertices_[0], vertices_[3]+vertices_[1])/2;
		}

		const std::vector<float> &GetVertices() const {
			return vertices_;
		}
		const bool IsVertical() const{
			if(abs(vertices_[0] - vertices_[2]) < abs(vertices_[1] - vertices_[3]))
				return true;
			else
				return false;
		}

		size_t GetSize() const {
			return vertices_count_;
		}

		double GetMaxValue() const {
			return *std::max_element(vertices_.begin(), vertices_.end());
		}
	};

	class Room : public MapItem {
	public:
		Room(Point &top_left, Point &bottom_right, std::string id) : MapItem(top_left, bottom_right, id) {
			size += 4;
			++count;
		}
		Room(Point top_left, Point bottom_right, Point entry, std::string id) :
								MapItem(top_left, bottom_right, id) {
			entry_ = entry;
			size += 4;
			++count;
		}
		Room(const std::vector<float> &verts) : MapItem(verts) {
			size += verts.size();
			++count;
		};

		size_t GetLength() const {
			return vertices_.size();
		}

		Point GetEntry() const {
		    return entry_;
		}

		virtual Point GetCenter() const override {
			if(entry_.x == vertices_[0] || entry_.x == vertices_[2]){
				return Point(MapItem::GetCenter().x, entry_.y);
			}else{
				return Point(entry_.x, MapItem::GetCenter().y);
			}
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
        Passage(Point top_left, Point bottom_right, std::string id) : MapItem(top_left, bottom_right, id) {
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
		Steps(Point top_left, Point bottom_right, std::string id) : Passage(top_left, bottom_right, id) {
			size += 4;
			++count_;
		}
	private:
		static size_t size;
		static size_t count_;

	};

	class Lift : public Passage {
	public:
		Lift(Point top_left, Point bottom_right, std::string id) : Passage(top_left, bottom_right, id) {
			size += 4;
			++count_;
		}
	private:
		static size_t size;
		static size_t count_;
	};
}

#endif //FOXMAPNATIVE_1_MAPITEM_H
