#pragma once
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include "FoxUtilites/Point.h"
#include "Database/Entity.h"
#include "Database/DBMaster.h"
#include <sstream>
#include <system_error>
#include <exception>
#include <queue>
#ifdef __ANDROID__
    #include <android/asset_manager.h>
#endif
class DataBase : public DBMaster
{
public:
	DataBase() throw (std::system_error);

#ifdef __ANDROID__
	DataBase(AAssetManager*);
	DataBase(AAssetManager*, const std::string&);
	DataBase(AAssetManager*, const std::vector<std::string>&);
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

	//std::vector<RoomParcel> GetRooms();
	//std::vector<PassageParcel> GetPassages();
	std::vector<Room> GetRooms();
	std::vector<Hall> GetHalls();
	Point GetMapDimensions() const;
	class Converter{
	public:
		Point operator()(const Coordinate& coord){
			return Point(coord.x, coord.y);
		}
	};
private:
    bool parseFile(std::string&) throw(std::logic_error);
	/*std::list< std::vector<float> > rooms_;
	std::list< std::vector<float> > passages_;*/
	std::vector<Hall> passages_;
	std::vector<Room> rooms_;
	std::vector<char> room_types_;
	std::vector<std::string> db_names_;
	AAssetManager* asset_manager_;
	Point dimensions_;

	static const std::string filename_;
};

