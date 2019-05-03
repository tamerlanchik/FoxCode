#include "DataBase.h"

char DataBase::filename_[] = "map.txt";

DataBase::DataBase()
{
	std::ifstream f(filename_);
	assert(f.is_open());
	std::string temp;
	while (f >> temp) {
		if (temp == "ROOM") {
			std::cout << "\nGot room\n";
			std::vector<float> coords;
			coords.reserve(4);
			float t;
			while (f >> t) {
				coords.push_back(t);
			}
			rooms_.emplace_back(coords);
		}		
	}
}


DataBase::~DataBase()
{
}

size_t DataBase::GetRoomNumber() {
	return rooms_.size();
}
size_t DataBase::GetPassageNumber() {
	return passages_.size();
}

std::vector<DataBase::RoomParcel> DataBase::GetRooms() {
	std::vector<RoomParcel> parcels;
	parcels.reserve(rooms_.size());

	for (auto room : rooms_) {
		RoomParcel parcel;
		parcel.top_left_ = Point(room[0], room[1]);
		parcel.bottom_right = Point(room[2], room[3]);
		parcel.name_ = "123";
		parcel.id_ = 1;
		parcels.push_back(parcel);
	}

	return parcels;
}
std::vector<DataBase::PassageParcel> DataBase::GetPassages() {
	std::vector<PassageParcel> parcels;
	return parcels;
}