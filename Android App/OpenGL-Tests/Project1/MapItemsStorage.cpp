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
	data.~vector();
	std::vector<DataBase::PassageParcel> d = database_->GetPassages();
	for (DataBase::PassageParcel c : d) {
		Passage* room = new Passage(c.lines);
		passage_storage_.push_back(room);
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
	buffer_.clear();
	return getRooms();
}
float* MapItemStorage::GetPassages() {
	buffer_.clear();
	return getPassages();
}

float* MapItemStorage::GetObjects() {
	buffer_.clear();
	getPassages();
	float* d2 = getRooms();
	return d2;
}


float* MapItemStorage::getRooms() {
	rooms_buf_size_ = Room::GetCount() * 8;
	//rooms_buf_size_ = 8;
	buffer_.reserve(buffer_.size() + rooms_buf_size_);
	std::vector<float> verts;
	int k = 0;
	for (Room* i : room_storage_) {
		++k;
		//if (k != 1) break;
		verts = i->GetVertices();
		buffer_.push_back(verts[0]);
		buffer_.push_back(verts[1]);

		buffer_.push_back(verts[2]);
		buffer_.push_back(verts[1]);

		buffer_.push_back(verts[2]);
		buffer_.push_back(verts[3]);

		buffer_.push_back(verts[0]);
		buffer_.push_back(verts[3]);
		//std::copy(verts.begin(), verts.end(), std::back_inserter(buffer_));
	}
	return &buffer_[0];
}

float* MapItemStorage::getPassages() {
	passages_buf_size_ = Passage::GetSize();
	buffer_.reserve(buffer_.size() + passages_buf_size_);
	std::vector<float> verts;
	for (Passage* i : passage_storage_) {
		verts = i->GetVertices();
		std::copy(verts.begin(), verts.end(), std::back_inserter(buffer_));
	}
	return &buffer_[0];
}

void MapItemStorage::SetDatabase(DataBase* database) {
	database_ = database;
}