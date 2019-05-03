#pragma once
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <assert.h>
#include <iostream>

class DataBase
{
public:
	DataBase();
	~DataBase();
	size_t GetRoomNumber();
	size_t GetPassageNumber();
	//enum ObjectTypes { ROOM, PASSAGE, STEPS };
	struct Point {
		float x;
		float y;
		Point() {};
		Point(float _x, float _y) {
			x = _x;
			y = _y;
		}
	};
	struct Parcel {
		Point top_left_;
		Point bottom_right;
		unsigned int id_;
		Parcel() {};
	};
	struct PassageParcel : public Parcel {

	};
	struct RoomParcel : public Parcel {
		std::string name_;
		RoomParcel() {};
	};

	std::vector<RoomParcel> GetRooms();
	std::vector<PassageParcel> GetPassages();
private:
	std::list< std::vector<float> > rooms_;
	std::list< std::vector<float> > passages_;

	static char filename_[];
};

