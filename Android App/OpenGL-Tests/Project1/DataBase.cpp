#include "DataBase.h"

char DataBase::filename_[] = "map.txt";

DataBase::DataBase()
{
	std::ifstream f(filename_);
	assert(f.is_open());

	std::string type;
	size_t size;
	float data;
	while (f >> type) {
		if (type.find("ROOM")) {
			std::cout << "\nRead room\n";
		}
		else if (type.find("Passage")) {

		}
		else {
			std::cout << "Wrong object";
			break;
		}
		f >> size;
		std::cout << "\n" << type << "\n";
		std::vector<float> coords;
		coords.reserve(4 * size);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 4; j++) {
				f >> data;
				coords.push_back(data);
			}
		}
		rooms_.push_back(coords);
	}
	f.close();
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
		parcel.lines = room;
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