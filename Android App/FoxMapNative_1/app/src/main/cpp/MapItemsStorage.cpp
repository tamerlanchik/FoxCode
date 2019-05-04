#include "MapItemsStorage.h"

MapItemStorage::MapItemStorage() {
	cout << "MapItemStorage created\n";
};

MapItemStorage* MapItemStorage::Get() {
	static MapItemStorage instance_;
	/*if (instance_ == NULL) {
		cerr << "instance is NULL\n";
		instance_ = new MapItemStorage();
	}*/
	return &instance_;
}

bool MapItemStorage::InflateStorage() {
	if (!database_)
		return 0;
	std::vector<DataBase::RoomParcel> data = database_->GetRooms();
	for (DataBase::RoomParcel c : data) {
		Room* room = new Room(c.lines);
		room_storage_.push_back(room);
	}
	return 1;
}

float* MapItemStorage::GetRectangles() {
	/*const size_t vertex_count = 6;
	float vertex_data_[vertex_count] = { -1, -1, -1, 1, 0, 0 };
	buffer_.reserve(vertex_count);
	for (float i : vertex_data_)
		buffer_.push_back(i);*/
		/*buffer_.reserve(storage_.size() * (2*4));
		for (MapItem* i : storage_) {
			float* verts = i->GetVertices();
			for (int j = 0; j < 8; j++)
				buffer_.push_back(verts[j]);
		}*/
	float* v = &buffer_[0];
	return v;
}

float* MapItemStorage::GetRooms() {
	buffer_.reserve(Room::GetSize());
	std::vector<float> verts;
	for (Room* i : room_storage_) {
		verts = i->GetVertices();
		std::copy(verts.begin(), verts.end(), std::back_inserter(buffer_));
	}
	return &buffer_[0];
}

void MapItemStorage::SetDatabase(DataBase* database) {
	database_ = database;
}