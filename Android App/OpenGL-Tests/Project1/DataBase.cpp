#include "DataBase.h"

char DataBase::filename_[] = "map.txt";

DataBase::DataBase()
{
	std::ifstream file(filename_);
	assert(file.is_open());

	std::string s;
	std::getline(file, s, '\0');
	file.close();
	std::stringstream f;
	f << s;
	file.close();
	//https://stackoverflow.com/questions/132358/how-to-read-file-content-into-istringstream
	assert(parseFile(f));
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

Point DataBase::GetMapDimensions() const {
	return dimensions_;
}

bool DataBase::parseFile(std::stringstream& f) {
	//------ReadHeader-----
	f >> dimensions_.x >> dimensions_.y;
	if (!dimensions_.x || !dimensions_.y) {
		std::cout << "Cannot get dimensions\n";
		return false;
	}

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
			return false;
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
	return true;
}