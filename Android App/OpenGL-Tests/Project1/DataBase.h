#pragma once
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include "Point.h"
#include <sstream>
#include <system_error>
#include <exception>
#ifdef __ANDROID__
    #include <android/asset_manager.h>
#endif
class DataBase
{
public:
	DataBase() throw (std::system_error);

#ifdef __ANDROID__
	DataBase(AAssetManager*);
#endif
	~DataBase();
	size_t GetRoomNumber();
	size_t GetPassageNumber();
	struct Parcel {
		//Point top_left_;
		//Point bottom_right;
		//std::vector<Point> lines;
		std::vector<float> lines;
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
	Point GetMapDimensions() const;
private:
    bool parseFile(std::string&) throw(std::logic_error);
	std::list< std::vector<float> > rooms_;
	std::list< std::vector<float> > passages_;
	Point dimensions_;

	static char filename_[];
};
